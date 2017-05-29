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
                LEXICALS,
                DONE
            };
        }


        template <typename CHAR>
        class lexer
        {
        private:
            core::array< ident_t<CHAR> >* input_ident;

            core::array< phrase_t<CHAR>* > phrase_nodes;

            phrase_t<CHAR>* current_node;

            int index;
            bool found_error;

            lex::progress progress;
            bool input_in_use;

            ///debug
            void print_lex_ast();
            void print_lx_ch(int, phrase_t<CHAR>*);

            void del_nodes(phrase_t<CHAR>* root)
            {
                for (int i=0; i<(root->children.size()); i++)
                {
                    del_nodes(root->children[i]);
                }

                delete root;
            }


            ///phrase detection
            bool found_mul_expr();
            bool found_add_expr();

            ///phrase concatenation
            void concat_mul_expr();
            void concat_add_expr();

        public:
            lexer()
            {
                input_ident = NULL;
                progress = lex::NONE;

                index = 0;
                found_error = false;

                input_in_use = true;
            }

            ~lexer()
            {
                for (int i=0; i<phrase_nodes.size(); i++)
                    del_nodes(phrase_nodes[i]);
            }

            void setInput(core::array< ident_t<CHAR> >& identifiers)
            {
                input_ident = &identifiers;

                for (int i=0; i<phrase_nodes.size(); i++)
                    del_nodes(phrase_nodes[i]);
                phrase_nodes.clear();

                progress = lex::NONE;

                index = 0;
                found_error = false;
            }


            bool lex(const core::timeout&);

            inline bool error() {return found_error;}

            inline bool usingInput() {return input_in_use;}
        };


        template <typename CHAR>
        bool lexer<CHAR>::lex(const core::timeout& time)
        {
            while (!time.timedOut() && (progress != lex::DONE))
            {
                if (progress == lex::NONE)
                {
                    phrase_nodes.add(new phrase_t<CHAR>(input_ident->at(index)));

                    index++;

                    if (index >= input_ident->size())
                    {
                        progress = lex::LEXICALS;
                        index = 0;

                        input_in_use = false;
                    }
                }
                else if (progress == lex::LEXICALS)
                {
                    if (index >= phrase_nodes.size())
                    {
                        progress = lex::DONE;
                        index = 0;
                    }
                    else if (found_mul_expr())
                        concat_mul_expr();
                    else if (found_add_expr())
                        concat_add_expr();
                    else
                        index++;
                }
            }

            ///debug
            if (progress == lex::DONE)
                print_lex_ast();

            return !time.timedOut();
        }



        ///phrase detection

        template <typename CHAR>
        bool lexer<CHAR>::found_mul_expr()
        {
            if ((index+2) >= phrase_nodes.size())
                return false;
            else
                return (phrase_nodes[index+1]->type == ident::OPER_MUL) ||
                       (phrase_nodes[index+1]->type == ident::OPER_DIV) ||
                       (phrase_nodes[index+1]->type == ident::OPER_IDIV) ||
                       (phrase_nodes[index+1]->type == ident::OPER_MOD);
        }

        template <typename CHAR>
        bool lexer<CHAR>::found_add_expr()
        {
            if ((index+2) >= phrase_nodes.size())
                return false;
            else
                return (phrase_nodes[index+1]->type == ident::OPER_ADD) ||
                       (phrase_nodes[index+1]->type == ident::OPER_SUB);
        }


        ///phrase concatenation

        template <typename CHAR>
        void lexer<CHAR>::concat_mul_expr()
        {
            phrase_t<CHAR>* mul_node = new phrase_t<CHAR>();

            mul_node->type = phrase::MUL_EXPR;

            mul_node->line = phrase_nodes[index]->line;
            mul_node->column = phrase_nodes[index]->column;

            phrase_nodes[index]->parent = mul_node;
            phrase_nodes[index+1]->parent = mul_node;
            phrase_nodes[index+2]->parent = mul_node;

            mul_node->children.add(phrase_nodes[index]);
            mul_node->children.add(phrase_nodes[index+1]);
            mul_node->children.add(phrase_nodes[index+2]);

            phrase_nodes.replace(index, index+2, mul_node);

            mul_node->err = error::NONE;

            index = 0;
        }

        template <typename CHAR>
        void lexer<CHAR>::concat_add_expr()
        {
            phrase_t<CHAR>* add_node = new phrase_t<CHAR>();

            add_node->type = phrase::ADD_EXPR;

            add_node->line = phrase_nodes[index]->line;
            add_node->column = phrase_nodes[index]->column;

            phrase_nodes[index]->parent = add_node;
            phrase_nodes[index+1]->parent = add_node;
            phrase_nodes[index+2]->parent = add_node;

            add_node->children.add(phrase_nodes[index]);
            add_node->children.add(phrase_nodes[index+1]);
            add_node->children.add(phrase_nodes[index+2]);

            phrase_nodes.replace(index, index+2, add_node);

            add_node->err = error::NONE;

            index = 0;
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
