/**
 * File:            add1expr.h
 * Namespace:       z::script
 * Description:     Implementation of the lexer's
 *                  syntax generating member functions.
 *                  This file should NOT be included
 *                  in a project. Include "lexer.h"
 *                  instead,and put this file in the
 *                  "syntaxRules" folder.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   15 Jul. 2017
**/

#pragma once
#ifndef ADD1EXPR_H_INCLUDED
#define ADD1EXPR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        bool lexer<CHAR>::add1expr()
        {
            if (phrase_nodes[index]->type == phrase::FACTORIALEXPR)
            {
                if (phrase_nodes.is_valid(index+1) &&
                    ((phrase_nodes[index+1]->type == ident::OPER_ADD1) ||
                     (phrase_nodes[index+1]->type == ident::OPER_SUB1)))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::ADD1EXPR;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    node->children.add(phrase_nodes[index]);
                    node->children.add(phrase_nodes[index+1]);

                    phrase_nodes[index]->parent = node;
                    phrase_nodes[index+1]->parent = node;

                    node->file = phrase_nodes[index]->file;

                    phrase_nodes.replace(index, index+1, node);

                    return true;
                }
                else if (phrase_nodes.is_valid(index-1) &&
                         ((phrase_nodes[index-1]->type == ident::OPER_ADD1) ||
                          (phrase_nodes[index-1]->type == ident::OPER_SUB1)))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::ADD1EXPR;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    node->children.add(phrase_nodes[index-1]);
                    node->children.add(phrase_nodes[index]);

                    phrase_nodes[index-1]->parent = node;
                    phrase_nodes[index]->parent = node;

                    phrase_nodes.replace(index-1, index, node);

                    return true;
                }
                else
                {
                    if (phrase_nodes[index]->orig_type == ident::NONE)
                        phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                    phrase_nodes[index]->type = phrase::ADD1EXPR;

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // ADD1EXPR_H_INCLUDED
