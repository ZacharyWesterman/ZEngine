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
 * Last modified:   23 Aug. 2017
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
    namespace compiler
    {
        template <typename CHAR>
        class constantFolder
        {
        private:
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
            void operate_powerexpr();
            void operate_factorialexpr();
            void operate_boolexpr();
            void operate_list();
            void operate_exprlist();
            void operate_add1expr();
            void operate_varindex();

        public:
            core::array< parserError<CHAR> > error_buffer;

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
                else if (root->type == phrase::POWEREXPR)
                    operate_powerexpr();
                else if (root->type == phrase::FACTORIALEXPR)
                    operate_factorialexpr();
                else if (root->type == phrase::BOOLEXPR)
                    operate_boolexpr();
                else if (root->type == phrase::LIST)
                    operate_list();
                else if (root->type == phrase::ADD1EXPR)
                    operate_add1expr();
                else if (root->type == phrase::VARINDEX)
                    operate_varindex();
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
                error_buffer.add(parserError<CHAR>(
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
                else
                    root->value = (root->children[0]->value).int_divide\
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
        void constantFolder<CHAR>::operate_powerexpr()
        {
            if ((root->children[0]->type == ident::LITERAL) &&
                (root->children[1]->type == ident::LITERAL))
            {
                root->value = (root->children[0]->value) ^
                              (root->children[1]->value);

                set_node_constant();
                append_oper_error();

                exit_node();
            }
            else if (index < 1)
                enter_node(0);
            else if (index < 2)
                enter_node(1);
            else
                exit_node();
        }


        template <typename CHAR>
        void constantFolder<CHAR>::operate_factorialexpr()
        {
            if (root->children[0]->type == ident::LITERAL)
            {
                root->value = (root->children[0]->value).factorial();

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
        void constantFolder<CHAR>::operate_boolexpr()
        {
            if ((root->children.size() > 2) &&
                (root->children[0]->type == ident::LITERAL) &&
                (root->children[2]->type == ident::LITERAL))
            {
                if (root->children[1]->type == ident::OPER_EQ)
                    root->value = (root->children[0]->value) ==
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_NOT_EQ)
                    root->value = (root->children[0]->value) !=
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_GT)
                    root->value = (root->children[0]->value) >
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_GT_EQ)
                    root->value = (root->children[0]->value) >=
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_LT)
                    root->value = (root->children[0]->value) <
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_LT_EQ)
                    root->value = (root->children[0]->value) <=
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_AND_LGCL)
                    root->value = (root->children[0]->value) &&
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_AND_BITW)
                    root->value = (root->children[0]->value) &
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_OR_LGCL)
                    root->value = (root->children[0]->value) ||
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_OR_BITW)
                    root->value = (root->children[0]->value) |
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_XOR_LGCL)
                    root->value = (root->children[0]->value).xor_logical\
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_XOR_BITW)
                    root->value = (root->children[0]->value).xor_bitwise\
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_NAND_LGCL)
                    root->value = (root->children[0]->value).nand_logical\
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_NAND_BITW)
                    root->value = (root->children[0]->value).nand_bitwise\
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_NOR_LGCL)
                    root->value = (root->children[0]->value).nor_logical\
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_NOR_BITW)
                    root->value = (root->children[0]->value).nor_bitwise\
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_NXOR_LGCL)
                    root->value = (root->children[0]->value).nxor_logical\
                                  (root->children[2]->value);
                else if (root->children[1]->type == ident::OPER_NXOR_BITW)
                    root->value = (root->children[0]->value).nxor_bitwise\
                                  (root->children[2]->value);

                set_node_constant();
                append_oper_error();

                exit_node();
            }
            else if (root->children[0]->type == ident::LITERAL)
            {
                if (root->children[1]->type == ident::OPER_NOT_BITW)
                    root->value = ~(root->children[0]->value);
                else
                    root->value = !(root->children[0]->value);

                set_node_constant();
                append_oper_error();

                exit_node();
            }
            else if (index < 1)
                enter_node(0);
            else if (index < (root->children.size() - 1))
                enter_node(2);
            else
                exit_node();
        }


        template <typename CHAR>
        void constantFolder<CHAR>::operate_list()
        {

            if (root->children.size() <= 0)
            {
                root->value = core::array< data_t<CHAR> >{};

                set_node_constant();
                append_oper_error();

                exit_node();
            }
            else if (index >= (root->children.size()))
            {
                bool found_nonliteral = false;
                int i = 0;
                while((i<(root->children.size())) &&
                      !found_nonliteral)
                {
                    if (root->children[i]->type != ident::LITERAL)
                    {
                        found_nonliteral = true;
                    }

                    i++;
                }

                if (!found_nonliteral)
                {
                    root->value = core::array< data_t<CHAR> > {root->children[0]->value};

                    for(int i=1; i<(root->children.size()); i++)
                        root->value.array().add(root->children[i]->value);

                    set_node_constant();
                    append_oper_error();
                }

                exit_node();
            }
            else
                enter_node(index);
        }


        template <typename CHAR>
        void constantFolder<CHAR>::operate_add1expr()
        {
            if (root->children[0]->type == ident::LITERAL)
            {
                if (root->children[1]->type == ident::OPER_ADD1)
                    root->value = ++(root->children[0]->value);
                else
                    root->value = --(root->children[0]->value);

                set_node_constant();
                append_oper_error();

                exit_node();
            }
            else if (root->children[1]->type == ident::LITERAL)
            {
                if (root->children[0]->type == ident::OPER_ADD1)
                    root->value = ++(root->children[1]->value);
                else
                    root->value = --(root->children[1]->value);

                set_node_constant();
                append_oper_error();

                exit_node();
            }
            else if (index < 1)
                enter_node(0);
            else if (index < 2)
                enter_node(1);
            else
                exit_node();
        }


        template <typename CHAR>
        void constantFolder<CHAR>::operate_varindex()
        {
            phrase_t<CHAR>* child[] =
            {
                root->children[0],
                root->children[1]
            };

            //look for LITERAL[LITERAL]
            if ((child[0]->type == ident::LITERAL) &&
                (child[1]->type == ident::LITERAL))
            {
                bool subIndex = (bool)root->metaValue;

                data_t<CHAR> value = root->children[0]->value;
                data_t<CHAR> v_index = root->children[1]->value;


                phrase_t<CHAR>* node = root->children[0];
                node->parent = root->parent;
                root->parent->children[index_stack.peek()-1] = node;

                delete root->children[1];
                delete root;

                root = node;



                data_t<CHAR> result;

                if (subIndex) //sub-indexing
                    result = value.subIndex(v_index);
                else //regular indexing
                    result = value.index(v_index);

                if (result.error())
                {
                    error_buffer.add(parserError<CHAR>(
                                            root->line,
                                            root->column,
                                            result.error(),
                                            root->file));
                }
                else
                {
                    root->value = result;
                }

                exit_node();
            }
            //look for LITERAL[range:LITERAL]
            else if ((child[0]->type == ident::LITERAL) &&
                     (child[1]->type == phrase::RANGE) &&
                     (child[1]->children[0]->type == ident::LITERAL) &&
                     (child[1]->children[1]->type == ident::LITERAL))
            {
                bool subIndex = (bool)root->metaValue;

                data_t<CHAR> value = child[0]->value;
                data_t<CHAR> v_start = child[1]->children[0]->value;
                data_t<CHAR> v_stop =  child[1]->children[1]->value;

                phrase_t<CHAR>* node = child[0];
                node->parent = root->parent;
                root->parent->children[index_stack.peek()-1] = node;

                delete child[1];
                delete root;

                root = node;



                data_t<CHAR> result;

                if (subIndex) //sub-indexing
                    result = value.subIndex(v_start, v_stop);
                else //regular indexing
                    result = value.index(v_start, v_stop);

                if (result.error())
                {
                    error_buffer.add(parserError<CHAR>(
                                            root->line,
                                            root->column,
                                            result.error(),
                                            root->file));
                }
                else
                {
                    root->value = result;
                }

                exit_node();
            }
            //look for LITERAL[index-list:LITERAL]
            else if ((child[0]->type == ident::LITERAL) &&
                     (child[1]->type == phrase::INDEXLIST))
            {
                if (child[0]->value.type() <= data::VALUE)
                {
                    error_buffer.add(parserError<CHAR>(child[0]->line,
                                                       child[0]->column,
                                                       error::CANNOT_INDEX,
                                                       child[0]->file));
                    exit_node();
                }
                else
                {
                    bool all_literal = true;
                    bool all_valid = true;

                    //check all indexes in the list
                    for(int i=0; i<(child[1]->children.size()); i++)
                    {
                        phrase_t<CHAR>* ptr = child[1]->children[i];

                        //if this index is a single index
                        if (ptr->type == ident::LITERAL)
                        {
                            if (ptr->value.type() != data::VALUE)
                            {
                                error_buffer.add(parserError<CHAR>(
                                                        ptr->line,
                                                        ptr->column,
                                                        error::ILLEGAL_INDEX,
                                                        ptr->file));

                                all_valid = false;
                            }
                        }
                        //if this index is a range
                        else if ((ptr->type == phrase::RANGE) &&
                             (ptr->children[0]->type == ident::LITERAL) &&
                             (ptr->children[1]->type == ident::LITERAL))
                        {
                            if (ptr->children[0]->value.type() != data::VALUE)
                            {
                                error_buffer.add(parserError<CHAR>(
                                                    ptr->children[0]->line,
                                                    ptr->children[0]->column,
                                                    error::ILLEGAL_INDEX,
                                                    ptr->children[0]->file));

                                all_valid = false;
                            }

                            if (ptr->children[1]->value.type() != data::VALUE)
                            {
                                error_buffer.add(parserError<CHAR>(
                                                    ptr->children[1]->line,
                                                    ptr->children[1]->column,
                                                    error::ILLEGAL_INDEX,
                                                    ptr->children[1]->file));

                                all_valid = false;
                            }
                        }
                        else
                        {
                            all_literal = false;
                        }
                    }


                    if (!all_literal)
                    {
                        if (index < 2)
                            enter_node(1);
                        else
                            exit_node();
                    }
                    else if (all_valid)
                    {
                        phrase_t<CHAR>* node = child[0];
                        node->parent = root->parent;
                        root->parent->children[index_stack.peek()-1] = node;

                        delete root;

                        root = node;

                        data_t<CHAR> result;
                        result.setType(root->value.type());


                        for(int i=0; i<(child[1]->children.size()); i++)
                        {
                            phrase_t<CHAR>* ptr = child[1]->children[i];


                            //if this index is a single index
                            if (ptr->type == ident::LITERAL)
                            {
                                int var_index;

                                var_index = (int)(ptr->value.real());

                                if (root->value.type() == data::ARRAY)
                                {
                                    if (root->value.array().
                                              is_valid(var_index))
                                        result.array().add(root->
                                            value.array().at(var_index));
                                    else
                                        error_buffer.add(parserError<CHAR>(
                                                            child[1]->line,
                                                            child[1]->column,
                                                error::INDEX_OUT_OF_BOUNDS,
                                                            child[1]->file));
                                }
                                else //STRING
                                {
                                    result = result +
                                    core::string<CHAR>(
                                    root->value.string().at(var_index));
                                }
                            }
                            //if this index is a range
                            else if (ptr->type == phrase::RANGE)
                            {
                                int start_index, stop_index;

                                start_index = (int)(ptr->
                                                    children[0]->
                                                    value.real());
                                stop_index = (int)(ptr->
                                                    children[1]->
                                                    value.real());

                                if (root->value.type() == data::ARRAY)
                                {
                                    if (root->value.array().
                                              is_valid(start_index) &&
                                        root->value.array().
                                              is_valid(stop_index))
                                    {
                                        result.array().add(root->
                                            value.array().subset(start_index,
                                                                 stop_index));
                                    }
                                    else
                                        error_buffer.add(parserError<CHAR>(
                                                            child[1]->line,
                                                            child[1]->column,
                                                error::INDEX_OUT_OF_BOUNDS,
                                                            child[1]->file));
                                }
                                else //STRING
                                {
                                    result = result +
                                        root->value.string().
                                        substr(start_index, stop_index);
                                }
                            }

                        }


                        root->value = result;

                        delete_ast(child[1]);

                        exit_node();
                    }
                    else
                        exit_node();
                }

            }
            else if (index < root->children.size())
                enter_node(index);
            else
                exit_node();
        }

    }
}
}

#endif // CONSTANTFOLDER_H_INCLUDED
