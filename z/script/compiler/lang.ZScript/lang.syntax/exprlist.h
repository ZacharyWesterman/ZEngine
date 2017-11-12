/**
 * File:            exprlist.h
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
 * Last modified:   27 Jul. 2017
**/

#pragma once
#ifndef EXPRLIST_H_INCLUDED
#define EXPRLIST_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class exprlist : public syntaxRule
        {
        public:
            ~exprlist() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*);
        };

        bool exprlist::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer)
        {
            if (phrase_nodes->is_valid(index+2))
            {
                if ((phrase_nodes->at(index)->type == BOOLEXPR) &&
                    (phrase_nodes->at(index+1)->type == ident::COMMA) &&
                    (phrase_nodes->at(index+2)->type == BOOLEXPR))
                {
                    phrase_t* node =
                        new phrase_t(*(phrase_nodes->at(index)), EXPRLIST);

                    phrase_nodes->at(index)->parent = node;
                    phrase_nodes->at(index+2)->parent = node;

                    node->children.add(phrase_nodes->at(index));
                    node->children.add(phrase_nodes->at(index+2));

                    delete phrase_nodes->at(index+1);
                    phrase_nodes->replace(index, index+2, node);

                    return true;
                }
                else if ((phrase_nodes->at(index)->type == EXPRLIST) &&
                         (phrase_nodes->at(index+1)->type == ident::COMMA) &&
                         (phrase_nodes->at(index+2)->type == BOOLEXPR))
                {
                    phrase_nodes->at(index+2)->parent = phrase_nodes->at(index);

                    phrase_nodes->at(index)->children.add(phrase_nodes->at(index+2));


                    delete phrase_nodes->at(index+1);
                    phrase_nodes->remove(index+2);
                    phrase_nodes->remove(index+1);

                    return true;
                }
                else if ((phrase_nodes->at(index)->type == EXPRLIST) &&
                         (phrase_nodes->at(index+1)->type == ident::COMMA) &&
                          (phrase_nodes->at(index+2)->type == EXPRLIST))
                {
                    for (int i=0; i<(phrase_nodes->at(index+2)->children.size()); i++)
                        phrase_nodes->at(index+2)->children[i]->parent = phrase_nodes->at(index);

                    phrase_nodes->at(index)->children.add(phrase_nodes->at(index+2)->children);


                    delete phrase_nodes->at(index+2);
                    delete phrase_nodes->at(index+1);
                    phrase_nodes->remove(index+2);
                    phrase_nodes->remove(index+1);

                    return true;
                }
                else if ((phrase_nodes->at(index)->type == BOOLEXPR) &&
                         (phrase_nodes->at(index+1)->type == ident::COMMA) &&
                          (phrase_nodes->at(index+2)->type == EXPRLIST))
                {
                    phrase_nodes->at(index)->parent = phrase_nodes->at(index+2);

                    phrase_nodes->at(index+2)->children.insert(phrase_nodes->at(index), 0);


                    delete phrase_nodes->at(index+1);
                    phrase_nodes->remove(index+1);
                    phrase_nodes->remove(index);

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // EXPRLIST_H_INCLUDED
