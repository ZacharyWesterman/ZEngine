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
        class add1expr : public syntaxRule
        {
        public:
            ~add1expr() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*) const;
        };

        bool add1expr::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer) const
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
                    phrase_t* node =
                        new phrase_t(*(phrase_nodes->at(index)),
                                           ADD1EXPR);

                    node->children.add(phrase_nodes->at(index));
                    node->children.add(phrase_nodes->at(index+1));

                    phrase_nodes->at(index)->parent = node;
                    phrase_nodes->at(index+1)->parent = node;


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
                    phrase_t* node =
                        new phrase_t(*(phrase_nodes->at(index)),
                                           ADD1EXPR);

                    node->children.add(phrase_nodes->at(index-1));
                    node->children.add(phrase_nodes->at(index));

                    phrase_nodes->at(index-1)->parent = node;
                    phrase_nodes->at(index)->parent = node;

                    phrase_nodes->replace(index-1, index, node);

                    return true;
                }
                else
                {
                    setSuperType(phrase_nodes->at(index), ADD1EXPR);

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // ADD1EXPR_H_INCLUDED
