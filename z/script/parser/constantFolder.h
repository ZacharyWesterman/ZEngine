/**
 * File:            constantFolder.h
 * Namespace:       z::script
 * Description:     A template class for optimizing a given
 *                  syntax tree via constant folding. The
 *                  folding is performed incrementally so
 *                  that script AST(s) can be optimized during
 *                  run-time without making the program
 *                  unresponsive.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   19 Jul. 2017
**/

#pragma once
#ifndef CONSTANTFOLDER_H_INCLUDED
#define CONSTANTFOLDER_H_INCLUDED

#include <z/core/dynamicStack.h>
#include <z/core/timeout.h>
#include "../errors.h"

#include "phrase.h"

#ifndef NULL
    #define NULL 0
#endif // NULL

namespace z
{
    namespace script
    {
        template <typename CHAR>
        class constantFolder
        {
        private:
            core::array< parser_error<CHAR> > error_buffer;

            phrase_t<CHAR>* root;

            int index;
            core::dynamicStack<int> index_stack;

            bool is_done;

            void enter_node(int);
            void exit_node();

            void set_node_constant();

            void append_oper_error();


            void operate_negatexpr();
            void operate_addexpr();
            void operate_multiplyexpr();

        public:
            constantFolder()
            {
                index = 0;
                root = NULL;

                is_done = true;
            };

            ~constantFolder(){};


            void setInput(phrase_t<CHAR>* new_root)
            {
                index_stack.dump();

                index = 0;
                root = new_root;

                is_done = (root == NULL);
            }

            inline bool error() {return (error_buffer.size() > 0);}

            inline bool done() {return is_done;}


            bool fold(const core::timeout&);
        };



        template <typename CHAR>
        bool constantFolder<CHAR>::fold(const core::timeout& time)
        {
            while (!is_done && !time.timedOut())
            {
                if (root->type == phrase::NEGATEXPR)
                    operate_negatexpr();
                else if (root->type == phrase::ADDEXPR)
                    operate_addexpr();
                else if (root->type == phrase::MULTIPLYEXPR)
                    operate_multiplyexpr();
                else
                {
                    if (index >= (root->children).size())
                    {
                        exit_node();
                    }
                    else
                    {
                        enter_node(index);
                    }
                }
            }

            return is_done;
        }


        template <typename CHAR>
        void constantFolder<CHAR>::enter_node(int entry)
        {
            root = root->children[entry];
            index_stack.push(entry+1);

            index = 0;
        }

        template <typename CHAR>
        void constantFolder<CHAR>::exit_node()
        {
            if (!index_stack.pop(index))
                is_done = true;
            else
                root = root->parent;
        }

        template <typename CHAR>
        void constantFolder<CHAR>::set_node_constant()
        {
            root->type = ident::LITERAL;
            root->meta = NULL;

            for (int i=0; i<(root->children).size(); i++)
                delete_ast(root->children[i]);

            root->children.clear();
        }


        template <typename CHAR>
        void constantFolder<CHAR>::append_oper_error()
        {
            if (root->value.type() == data::ERROR)
                error_buffer.add(parser_error<CHAR>(
                                        root->line,
                                        root->column,
                                        root->value.error(),
                                        root->file));
        }



        template <typename CHAR>
        void constantFolder<CHAR>::operate_negatexpr()
        {
            if (root->children[0]->type == ident::LITERAL)
            {
                root->value = -(root->children[0]->value);

                set_node_constant();
                append_oper_error();

                exit_node();
            }
            else if (index < 1)
                enter_node(0);
            else
                exit_node();
        }


        template <typename CHAR>
        void constantFolder<CHAR>::operate_addexpr()
        {
            if ((root->children[0]->type == ident::LITERAL) &&
                (root->children[2]->type == ident::LITERAL))
            {
                if (root->children[1]->type == ident::OPER_ADD)
                    root->value = (root->children[0]->value) +
                                  (root->children[2]->value);
                else
                    root->value = (root->children[0]->value) -
                                  (root->children[2]->value);

                set_node_constant();
                append_oper_error();

                exit_node();
            }
            else if (index < 1)
                enter_node(0);
            else if (index < 3)
                enter_node(2);
            else
                exit_node();
        }


        template <typename CHAR>
        void constantFolder<CHAR>::operate_multiplyexpr()
        {
            if ((root->children[0]->type == ident::LITERAL) &&
                (root->children[2]->type == ident::LITERAL))
            {
                if (root->children[1]->type == ident::OPER_MUL)
                    root->value = (root->children[0]->value) *
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_DIV)
                    root->value = (root->children[0]->value) /
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_MOD)
                    root->value = (root->children[0]->value) %
                                  (root->children[2]->value);

                set_node_constant();
                append_oper_error();

                exit_node();
            }
            else if (index < 1)
                enter_node(0);
            else if (index < 3)
                enter_node(2);
            else
                exit_node();
        }
    }
}

#endif // CONSTANTFOLDER_H_INCLUDED
