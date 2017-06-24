/**
 * File:            scan_iterator.h
 * Namespace:       z::script
 * Description:     A controller for the scanner that
 *                  replaces "include" statements with
 *                  the script in the specified file.
 *                  This process is performed in increments
 *                  so that scripts can be compiled during
 *                  runtime without making the program
 *                  unresponsive.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   14 May. 2017
**/

#pragma once
#ifndef INCLUDEITERATOR_H_INCLUDED
#define INCLUDEITERATOR_H_INCLUDED

#include <z/core/array.h>
#include <z/file/loadFileTimeout.h>
#include <z/file/directoryOperations.h>
#include <z/core/timeout.h>

#include "scanner.h"
#include "lexer.h"


#include <iostream>
using namespace std;

namespace z
{
    namespace script
    {
        enum iter_progress
        {
            PROG_NONE = 0,
            PROG_LOADING,
            PROG_LOADED,
            PROG_SCAN_READY,
            PROG_SCANNING,
            PROG_SCANNED,
            PROG_LEX_READY,
            PROG_LEXING,
            PROG_LEXED,

            PROG_DONE
        };

        template <typename CHAR>
        class s_iter_node
        {
        public:

            core::string<char> directory;
            core::string<char> filename;

            core::string<CHAR> contents;

            core::array< ident_t<CHAR> > identities;

            core::array< parser_error<CHAR> > error_buffer;

            int insert_index;
            iter_progress progress;

            error_flag generic_error;

            //index of this node's file
            int file;

            s_iter_node()
            {
                progress = PROG_NONE;
                insert_index = -1;
                generic_error = error::NONE;

                file = -1;
            }

            bool operator==(const s_iter_node& other) const
            {
                return ((directory == other.directory) &&
                        (filename == other.filename) &&
                        (contents == other.contents) &&
                        (file == other.file));
            }

            core::string<CHAR> fullFileName()
            {
                core::string<CHAR> fullFName;
                if (directory.length())
                    fullFName = directory + '\\';

                if (filename.length())
                    fullFName += filename;
                else
                    fullFName += '.';

                return fullFName;
            }
        };


        template <typename CHAR>
        class includeIterator
        {
        private:
            core::array< core::string<CHAR> > file_list;

            core::string<CHAR>* full_output;

            file::loader<CHAR> fLoader;

            scanner<CHAR> fScanner;
            lexer<CHAR>   fLexer;

            bool found_error;

            core::array< s_iter_node<CHAR> > node_list;

            int working_node;

        public:


            includeIterator(core::sorted_ref_array< core::string<CHAR>* >* symbol_table) :
                            fScanner(symbol_table)
            {
                full_output = NULL;

                found_error = false;

                working_node = 0;
            }

            ~includeIterator()
            {

            }


            ///set the input string for the scan iterator
            void setInput(const core::string<CHAR>& input,
                          bool is_file = false)
            {
                node_list.clear();
                file_list.clear();
                working_node = 0;

                if (is_file)
                {
                    s_iter_node<CHAR> node;

                    int pos = input.findLast("/");
                    if (pos < 0)
                        pos = input.findLast("\\");

                    node.directory = file::shorten(input.substr(0, pos-1));
                    node.filename = input.substr(pos+1, input.length()-1);

                    node.file = file_list.size();
                    file_list.add(node.fullFileName());

                    node_list.add(node);
                }
                else
                {
                    s_iter_node<CHAR> node;
                    node.contents = input;
                    node.progress = PROG_SCAN_READY;

                    node_list.add(node);
                }
            }


            bool build(const core::timeout&);

            inline bool error() {return found_error;}

            void printErrors();
        };



        ///template for iterator scanner. Scans the loaded
        ///input and replaces all includes in the script
        ///with the contents of the referenced file.
        //returns 1 if done scanning.
        //returns 0 otherwise.
        template <typename CHAR>
        bool includeIterator<CHAR>::build(const core::timeout& time)
        {
            while ((working_node < node_list.size()) && !time.timedOut())
            {
                int progress = node_list[working_node].progress;

                if (progress == PROG_NONE)
                {

                    core::string<char> file = node_list[working_node].fullFileName();

                    fLoader.clear();
                    fLoader.setFileName(file);

                    node_list[working_node].progress = PROG_LOADING;
                }
                else if (progress == PROG_LOADING)
                {
                    int loadmsg = fLoader.load(time);

                    if (loadmsg == -1)
                    {
                        node_list[working_node].progress = PROG_DONE;

                        node_list[working_node].generic_error = error::INCLUDE_LOAD_FAILED;
                        found_error = true;
                    }
                    else if (loadmsg == 1)
                    {
                        node_list[working_node].progress = PROG_LOADED;
                    }
                }
                else if (progress == PROG_LOADED)
                {
                    node_list[working_node].contents = fLoader.getContents();
                    fLoader.clear();
                }
                else if (progress == PROG_SCAN_READY)
                {
                    fScanner.clear();
                    fScanner.file = file_list.size() - 1;
                    fScanner.setInput(node_list[working_node].contents);
                    fScanner.setOutput(node_list[working_node].identities);

                    node_list[working_node].progress = PROG_SCANNING;
                }
                else if (progress == PROG_SCANNING)
                {
                    if (fScanner.scan(time))
                        node_list[working_node].progress = PROG_SCANNED;
                }
                else if (progress == PROG_SCANNED)
                {
                    for(int i=0; i<node_list[working_node].identities.size()-1; i++)
                    {
                        if (node_list[working_node].identities[i].type == ident::KEYWORD_INCLUDE)
                        {
                            if (node_list[working_node].identities[i+1].type == ident::STRING_LITERAL)
                            {
                                core::string<char> full_fname = node_list[working_node].directory;
                                if (full_fname.length())
                                    full_fname += '\\';
                                full_fname += *(node_list[working_node].identities[i+1].meta);


                                s_iter_node<CHAR> node;

                                int pos = full_fname.findLast("/");
                                if (pos < 0)
                                    pos = full_fname.findLast("\\");

                                node.directory = file::shorten(full_fname.substr(0, pos-1));
                                node.filename = full_fname.substr(pos+1, full_fname.length()-1);


                                core::string<char> file = node_list[working_node].directory;


                                bool exists = false;
                                for (int i=0; i<node_list.size(); i++)
                                    if ((node_list[i].directory == node.directory) &&
                                        (node_list[i].filename == node.filename))
                                    {
                                        exists = true;
                                        break;
                                    }

                                if (exists)
                                {
                                    cout << "["<< node.directory.str() << ", ";
                                    cout << node.filename.str() << "]\n";
                                }
                                else
                                {
                                    node_list.add(node);
                                }


                            }
                            else
                            {
                                node_list[working_node].error_buffer.add(
                                    parser_error<CHAR>(node_list[working_node].identities[i].line,
                                                       node_list[working_node].identities[i].column,
                                                       error::INVALID_INCLUDE, node_list[working_node].file));

                                found_error = true;
                            }
                        }
                    }

                    found_error |= fScanner.error();

                    if (found_error)
                    {
                        node_list[working_node].error_buffer.add(fScanner.error_buffer);

                        node_list[working_node].progress = PROG_DONE;
                    }
                    else
                        node_list[working_node].progress = PROG_LEX_READY;
                }
                else if (progress == PROG_LEX_READY)
                {
                    fLexer.setInput(node_list[working_node].identities);

                    node_list[working_node].progress = PROG_LEXING;
                }
                else if (progress == PROG_LEXING)
                {
                    if (fLexer.lex(time))
                        node_list[working_node].progress = PROG_LEXED;
                }
                else if (progress == PROG_LEXED)
                {
                    found_error |= fLexer.error();

                    if (found_error)
                    {
                        node_list[working_node].error_buffer.add(fLexer.error_buffer);
                        found_error = true;

                        node_list[working_node].progress = PROG_DONE;
                    }
                    else
                        node_list[working_node].progress = PROG_DONE;
                }
                else if (progress == PROG_DONE)
                {
                    //cout << node_list[working_node].contents.str() << endl;

                    working_node++;
                }
            }


            return !time.timedOut();
        }


        ///debug
        template <typename CHAR>
        void includeIterator<CHAR>::printErrors()
        {
            if (found_error)
            {
                for (int i=0; i<node_list.size(); i++)
                {
                    for (int e=0; e<node_list[i].error_buffer.size(); e++)
                    {
                        parser_error<CHAR> perr = node_list[i].error_buffer[e];

                        cout << "Error ";

                        if (node_list[i].error_buffer[e].file > -1)
                            cout << "in \""
                                 << file_list[node_list[i].error_buffer[e].file].str()
                                 << "\" ";


                        cout << "at line " << perr.line
                             << ", column " << perr.column
                             << " : ";


                        switch (perr.err)
                        {
                        case error::INVALID_IDENTIFIER:
                            cout << "The symbol \"" << perr.extra_data.str() <<
                            "\" contains illegal characters.";
                            break;

                        case error::UNKNOWN_OPERATOR:
                            cout << "Unknown operator \"" << perr.extra_data.str() <<
                            "\".";
                            break;

                        case error::AMBIGUOUS_EXPR:
                            cout << "The expression \"" << perr.extra_data.str() <<
                            "\" contains illegal characters.";
                            break;

                        case error::SYNTAX_ERROR:
                            cout << "Syntax error.";
                            break;

                        default:
                            cout << "Unhandled error.";
                        }

                        cout << endl;
                    }
                }
            }
        }


    }
}

#endif // INCLUDEITERATOR_H_INCLUDED
