/**
 * File:            multiplyexpr.h
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
#ifndef MULTIPLYEXPR_H_INCLUDED
#define MULTIPLYEXPR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        class multiplyexpr : public syntaxRule
        {
        public:
            ~multiplyexpr() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*);
        };


        bool multiplyexpr::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer)
        {
            //if no detected multiplication operators, continue to the next phase
            if ((phrase_nodes->at(index)->type == POWEREXPR) &&
                !(phrase_nodes->is_valid(index+1) &&
                  (phrase_nodes->at(index+1)->type == ident::OPERATOR) &&
                  (phrase_nodes->at(index+1)->metaValue >= MUL) &&
                  (phrase_nodes->at(index+1)->metaValue <= MOD)
                  ) &&
                !(phrase_nodes->is_valid(index-1) &&
                  (phrase_nodes->at(index-1)->type == ident::OPERATOR) &&
                  (phrase_nodes->at(index-1)->metaValue >= MUL) &&
                  (phrase_nodes->at(index-1)->metaValue <= MOD)
                  )
                )
            {
                setSuperType(phrase_nodes->at(index), MULTIPLYEXPR);

                return true;
            }
            //otherwise, if a multiplication operator is detected
            else if (phrase_nodes->is_valid(index+2) &&
                     ((phrase_nodes->at(index)->type == POWEREXPR) ||
                      (phrase_nodes->at(index)->type == MULTIPLYEXPR)
                      ) &&
                     (phrase_nodes->at(index+1)->type == ident::OPERATOR) &&
                     (phrase_nodes->at(index+1)->metaValue >= MUL) &&
                     (phrase_nodes->at(index+1)->metaValue <= MOD) &&
                     (phrase_nodes->at(index+2)->type == POWEREXPR)
                     )
            {
                phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), MULTIPLYEXPR);

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+1)->parent = node;
                phrase_nodes->at(index+2)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+1));
                node->children.add(phrase_nodes->at(index+2));

                phrase_nodes->replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // MULTIPLYEXPR_H_INCLUDED
