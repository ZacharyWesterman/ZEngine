/**
 * File:            includeIterator.h
 * Namespace:       z::script
 * Description:     A partial script compiler that includes up
 *                  to scanning and lexical analysis. Additionally,
 *                  it replaces "include" statements with
 *                  the script in the specified file.
 *                  This process is performed in increments
 *                  so that scripts can be compiled during
 *                  run-time without making the program
 *                  unresponsive.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   20 Aug. 2017
**/

#pragma once
#ifndef INCLUDEITERATOR_H_INCLUDED
#define INCLUDEITERATOR_H_INCLUDED

#include <z/core/array.h>
#include <z/file/reader.h>
#include <z/file/shorten.h>
#include <z/file/exists.h>
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
            PROG_MERGE_READY,
            PROG_MERGING,
            PROG_LEX_READY,
            PROG_LEXING,
            PROG_LEXED,

            PROG_DONE
        };

        template <typename CHAR>
        class s_iter_node
        {
        public:
            int parent;

            core::string<char> directory;
            core::string<char> filename;

            core::string<CHAR> contents;

            core::array< ident_t<CHAR> > identities;

            core::array< parser_error<CHAR> > error_buffer;

            int insert_index;
            iter_progress progress;

            error_flag generic_error;

            //pointer to this node's file
            core::string<CHAR>* file;

            s_iter_node()
            {
                progress = PROG_NONE;
                insert_index = -1;
                generic_error = error::NONE;

                file = NULL;
                parent = -1;
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
            core::sortedRefArray< core::string<CHAR>* >* file_list;


            file::reader<CHAR> fLoader;

            scanner<CHAR> fScanner;
            lexer<CHAR>   fLexer;

            bool found_error;

            core::array< s_iter_node<CHAR> > node_list;

            int working_node;

            core::array< ident_t<CHAR> > full_ident_list;

            bool is_done;

            int overall_progress;

            void mergeIncludes();

        public:
            core::array< parser_error<CHAR> > error_buffer;

            includeIterator(core::sortedRefArray< core::string<CHAR>* >* symbol_table,
                            core::sortedRefArray< core::string<CHAR>* >* _file_list) :
                            fScanner(symbol_table)
            {

                found_error = false;

                working_node = 0;

                is_done = true;

                overall_progress = PROG_NONE;

                file_list = _file_list;
            }

            ~includeIterator()
            {

            }


            ///set the input string for the scan iterator
            void setInput(const core::string<CHAR>& input,
                          bool is_file = false)
            {
                node_list.clear();
                error_buffer.clear();

                working_node = 0;

                is_done = !file_list;
                overall_progress = PROG_NONE;

                if (is_file)
                {
                    s_iter_node<CHAR> node;

                    int pos = input.findLast("/");
                    if (pos < 0)
                        pos = input.findLast("\\");

                    node.directory = file::shorten(input.substr(0, pos-1));
                    node.filename = input.substr(pos+1, input.length()-1);

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

            inline bool done() {return is_done;}

            inline phrase_t<CHAR>* moveResultAST() {return fLexer.moveResultAST();}
        };



        ///template for iterator scanner. Scans the loaded
        ///input and replaces all includes in the script
        ///with the contents of the referenced file.
        //returns 1 if done scanning.
        //returns 0 otherwise.
        template <typename CHAR>
        bool includeIterator<CHAR>::build(const core::timeout& time)
        {
            while (!is_done && !time.timedOut())
            {
                if (overall_progress == PROG_NONE)
                {
                    int progress = node_list[working_node].progress;

                    //cout << working_node << " : " << progress << endl;
                    if (progress == PROG_NONE)
                    {

                        core::string<char> file = node_list[working_node].fullFileName();

                        fLoader.clear();
                        fLoader.set(file);

                        int fileID = file_list->find(&file);
                        if (fileID < 0)
                        {
                            fileID = file_list->add(new core::string<CHAR>(file));
                        }

                        node_list[working_node].file = file_list->at(fileID);

                        node_list[working_node].progress = PROG_LOADING;
                    }
                    else if (progress == PROG_LOADING)
                    {
                        int loadmsg = fLoader.read(time);

                        if (loadmsg == -1)
                        {
                            node_list[working_node].progress = PROG_MERGE_READY;
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

                        node_list[working_node].progress = PROG_SCAN_READY;
                    }
                    else if (progress == PROG_SCAN_READY)
                    {
                        fScanner.clear();
                        fScanner.file = node_list[working_node].file;
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
                        mergeIncludes();

                        found_error |= fScanner.error();

                        if (found_error)
                            node_list[working_node].error_buffer.add(fScanner.error_buffer);

                        node_list[working_node].progress = PROG_MERGE_READY;
                    }
                    else if (progress >= PROG_MERGE_READY)
                    {
                        if (working_node >= node_list.size() - 1)
                            overall_progress = PROG_MERGE_READY;
                        else
                            working_node++;
                    }

                    if (working_node >= node_list.size())
                        working_node = 0;
                }
                else if (overall_progress == PROG_MERGE_READY)
                {
                    if (found_error)
                    {
                        for (int i=0; i<node_list.size(); i++)
                            error_buffer.add(node_list[i].error_buffer);

                        overall_progress = PROG_DONE;
                    }
                    else
                    {
                        working_node = node_list.size() - 1;
                        overall_progress = PROG_MERGING;
                    }
                }
                else if (overall_progress == PROG_MERGING)
                {
                    if (working_node > 0)
                    {
                        int insert_index = node_list[working_node].insert_index;
                        int parent = node_list[working_node].parent;

                        node_list[parent].identities.replace(insert_index,
                                                             insert_index+1,
                                                             node_list[working_node].identities);

                        node_list.remove(working_node);
                        working_node--;
                    }
                    else
                        overall_progress = PROG_LEX_READY;
                }
                else if (overall_progress == PROG_LEX_READY)
                {
                    fLexer.setInput(node_list[0].identities);
                    overall_progress = PROG_LEXING;
                }
                else if (overall_progress == PROG_LEXING)
                {
                    if (fLexer.lex(time))
                        overall_progress = PROG_LEXED;

                    if (node_list.size() && !fLexer.usingInput())
                        node_list.clear();
                }
                else if (overall_progress == PROG_LEXED)
                {
                    found_error |= fLexer.error();

                    if (found_error)
                    {
                        error_buffer.add(fLexer.error_buffer);
                    }

                    overall_progress = PROG_DONE;
                }
                else if (overall_progress == PROG_DONE)
                {
                    is_done = true;
                }
            }


            return !time.timedOut();
        }


        template <typename CHAR>
        void includeIterator<CHAR>::mergeIncludes()
        {
            for(int i=0; i<node_list[working_node].identities.size(); i++)
            {
                if (node_list[working_node].identities[i].type == ident::KEYWORD_INCLUDE)
                {
                    if (node_list[working_node].identities.is_valid(i+1) &&
                        (node_list[working_node].identities[i+1].type == ident::LITERAL) &&
                        (node_list[working_node].identities[i+1].value.type() == data::STRING))
                    {
                        core::string<char> full_fname = node_list[working_node].directory;
                        if (full_fname.length())
                            full_fname += '\\';
                        full_fname += node_list[working_node].identities[i+1].value.string();


                        s_iter_node<CHAR> node;

                        int pos = full_fname.findLast("/");
                        if (pos < 0)
                            pos = full_fname.findLast("\\");

                        node.directory = file::shorten(full_fname.substr(0, pos-1));
                        node.filename = full_fname.substr(pos+1, full_fname.length()-1);

                        node.insert_index = i;
                        node.parent = working_node;

                        core::string<char> file = node_list[working_node].directory;


                        bool exists = false;

                        for (int j=0; j<node_list.size(); j++)
                        {
                            if ((node_list[j].directory == node.directory) &&
                                (node_list[j].filename == node.filename))
                            {
                                exists = true;
                                break;
                            }
                        }

                        if (exists)
                        {
                            node_list[working_node].identities.remove(i+1);
                            node_list[working_node].identities.remove(i);
                            i--;
                        }
                        else
                        {
                            if (file::exists(full_fname))
                            {
                                node_list.add(node);
                                i++;
                            }
                            else
                            {
                                node_list[working_node].error_buffer.add(
                                    parser_error<CHAR>(node_list[working_node].identities[i+1].line,
                                                        node_list[working_node].identities[i+1].column,
                                                        error::INCLUDE_LOAD_FAILED,
                                                        node_list[working_node].file));
                                found_error = true;

                                node_list[working_node].identities.remove(i+1);
                                node_list[working_node].identities.remove(i);
                                i--;
                            }
                        }


                    }
                    else
                    {
                        node_list[working_node].error_buffer.add(
                            parser_error<CHAR>(node_list[working_node].identities[i].line,
                                                node_list[working_node].identities[i].column,
                                                error::INVALID_INCLUDE,
                                                node_list[working_node].file));

                        found_error = true;

                        node_list[working_node].identities.remove(i);
                        i--;
                    }
                }
            }

        }


    }
}

#endif // INCLUDEITERATOR_H_INCLUDED
