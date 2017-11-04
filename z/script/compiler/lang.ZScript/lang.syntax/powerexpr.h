/**
 * File:            powerexpr.h
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
#ifndef POWEREXPR_H_INCLUDED
#define POWEREXPR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        class powerexpr : public syntaxRule
        {
        public:
            ~powerexpr() {}

            bool apply(core::array< phrase_t* >*,
                       int);
        };


        bool powerexpr::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index)
        {
            //if no detected power operators, continue to the next phase
            if ((phrase_nodes->at(index)->type == NEGATEXPR) &&
                !(phrase_nodes->is_valid(index+1) &&
                  (phrase_nodes->at(index+1)->type == ident::OPERATOR) &&
                  (phrase_nodes->at(index+1)->metaValue == POW)
                  ) &&
                !(phrase_nodes->is_valid(index-1) &&
                  (phrase_nodes->at(index-1)->type == ident::OPERATOR) &&
                  (phrase_nodes->at(index-1)->metaValue == POW)
                  )
                )
            {
                setSuperType(phrase_nodes->at(index), POWEREXPR);

                return true;
            }
            //otherwise, if a power operator is detected
            else if (phrase_nodes->is_valid(index+2) &&
                     ((phrase_nodes->at(index)->type == NEGATEXPR) ||
                      (phrase_nodes->at(index)->type == POWEREXPR)
                      ) &&
                     (phrase_nodes->at(index+1)->type == ident::OPERATOR) &&
                     (phrase_nodes->at(index+1)->metaValue == POW) &&
                     (phrase_nodes->at(index+2)->type == NEGATEXPR)
                     )
            {
                phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), POWEREXPR);

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+2)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+2));

                delete phrase_nodes->at(index+1);
                phrase_nodes->replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // POWEREXPR_H_INCLUDED
