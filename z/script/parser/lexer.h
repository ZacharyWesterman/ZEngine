/**
 * File:            lexer.h
 * Namespace:       z::script
 * Description:     Script lexer template. The lexer
 *                  takes the list of tokens generated
 *                  by the scanner and groups them into
 *                  functional phrases (e.g. expression,
 *                  scope, etc.)
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   6 Mar. 2017
**/


#pragma once
#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include <z/core/timeout.h>
#include <z/core/array.h>

#include "phrase.h"

#include <iostream>

namespace z
{
    namespace script
    {
        ///debug
        const char* symTypeStr[] =
        {
            "none",
            "(",")",
            "[","]",
            "{","}",
            ",",";",".",
            "#str","#num",
            "id",
            "func","cmd",
            "if","else",
            "for","each","do","loop","while",
            "goto","gosub",
            "run","include",
            "break","return",
            "exit",
            "wait","until",
            "type",
            "global","external",
            "=",
            "+","-",
            "++","--",
            "*","/","//","%",
            "^","!",
            "and","&",
            "or","|",
            "xor",":",
            "not","~",
            "nand","~&",
            "nor","~|",
            "xnor",
            "==","<>",">",">=","<","<=",
            "unknown",

            "add_expr",
            "mul_expr",
            "list"
        };

        namespace lex
        {
            enum progress
            {
                NONE = 0,
                GET_ADD_EXPRS,
                GET_MUL_EXPRS,
                DONE
            };
        }


        template <typename CHAR>
        class lexer
        {
        private:
            core::array< ident_t<CHAR> >* input_ident;

            core::array< phrase_t<CHAR> > phrase_list;

            int index;
            bool found_error;

            lex::progress progress;

            ///debug
            void print_lex_ast();
            void print_lx_ch(int, phrase_t<CHAR>*);

        public:
            lexer()
            {
                input_ident = NULL;
                progress = lex::NONE;

                index = 0;
                found_error = false;
            }

            void setInput(core::array< ident_t<CHAR> >& identifiers)
            {
                input_ident = &identifiers;
                phrase_list.clear();

                progress = lex::NONE;

                index = 0;
                found_error = false;
            }

            bool lex(const core::timeout&);

            inline bool error() {return found_error;}
        };


        template <typename CHAR>
        bool lexer<CHAR>::lex(const core::timeout& time)
        {
            while (!time.timedOut() && (progress != lex::DONE))
            {
                if (progress == lex::NONE)
                {
                    phrase_list.add(phrase_t<CHAR>());
                    phrase_list[index].data = input_ident->at(index);

                    index++;

                    if (index >= input_ident->size())
                    {
                        progress = lex::GET_MUL_EXPRS;
                        index = 0;
                    }
                }
                else if (progress == lex::GET_MUL_EXPRS)
                {
                    index++;

                    if (index >= phrase_list.size())
                    {
                        progress = lex::GET_ADD_EXPRS;
                        index = 0;
                    }
                }
                else if (progress == lex::GET_ADD_EXPRS)
                {
                    index++;

                    if (index >= phrase_list.size())
                    {
                        progress = lex::DONE;
                        index = 0;
                    }
                }
            }

            ///debug
            if (progress == lex::DONE)
                print_lex_ast();

            return !time.timedOut();
        }


        ///debug
        template <typename CHAR>
        void lexer<CHAR>::print_lex_ast()
        {
            for (int n=0; n<phrase_nodes.size(); n++)
            {
                print_lx_ch(0, phrase_nodes[n]);
            }
        }

        ///debug
        template <typename CHAR>
        void lexer<CHAR>::print_lx_ch(int level, phrase_t<CHAR>* node)
        {
            for (int i=0; i<level; i++)
                std::cout << "    ";



            if (node->type == ident::STRING_LITERAL)
                std::cout << "<" << ((core::string<CHAR>*)(node->meta))->str() << ">";
            else if (node->type == ident::NUMERIC_LITERAL)
                std::cout << "<" << node->value << ">";
            else
                std::cout << symTypeStr[node->type];

            std::cout << std::endl;

            for (int i=0; i<(node->children.size()); i++)
                print_lx_ch(level+1, node->children[i]);
        }
    }
}

#endif // LEXER_H_INCLUDED
