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
        class add1expr : public syntaxRule<CHAR>
        {
        public:
            ~add1expr() {}

            bool apply(core::array< phrase_t<CHAR>* >*,
                       int);
        };

        template <typename CHAR>
        bool add1expr<CHAR>::apply(core::array< phrase_t<CHAR>* >* phrase_nodes,
                                  int index)
        {
            if (phrase_nodes->at(index)->type == FACTORIALEXPR)
            {
                if (phrase_nodes->is_valid(index+1) &&
                    (phrase_nodes->at(index+1)->type == ident::OPERATOR) &&
                    ((phrase_nodes->at(index+1)->metaValue == ADD1) ||
                     (phrase_nodes->at(index+1)->metaValue == SUB1)
                     )
                    )
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = ADD1EXPR;

                    node->line = phrase_nodes->at(index)->line;
                    node->column = phrase_nodes->at(index)->column;

                    node->children.add(phrase_nodes->at(index));
                    node->children.add(phrase_nodes->at(index+1));

                    phrase_nodes->at(index)->parent = node;
                    phrase_nodes->at(index+1)->parent = node;

                    node->file = phrase_nodes->at(index)->file;

                    phrase_nodes->replace(index, index+1, node);

                    return true;
                }
                else if (phrase_nodes->is_valid(index-1) &&
                         (phrase_nodes->at(index-1)->type == ident::OPERATOR) &&
                         ((phrase_nodes->at(index-1)->metaValue == ADD1) ||
                          (phrase_nodes->at(index-1)->metaValue == SUB1)
                          )
                         )
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = ADD1EXPR;

                    node->line = phrase_nodes->at(index)->line;
                    node->column = phrase_nodes->at(index)->column;

                    node->children.add(phrase_nodes->at(index-1));
                    node->children.add(phrase_nodes->at(index));

                    phrase_nodes->at(index-1)->parent = node;
                    phrase_nodes->at(index)->parent = node;

                    phrase_nodes->replace(index-1, index, node);

                    return true;
                }
                else
                {
                    if (phrase_nodes->at(index)->orig_type == ident::NONE)
                        phrase_nodes->at(index)->orig_type = phrase_nodes->at(index)->type;

                    phrase_nodes->at(index)->type = ADD1EXPR;

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // ADD1EXPR_H_INCLUDED
