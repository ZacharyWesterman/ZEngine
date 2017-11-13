/**
 * File:            negatexpr.h
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
#ifndef NEGATEXPR_H_INCLUDED
#define NEGATEXPR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        class negatexpr : public syntaxRule
        {
        public:
            ~negatexpr() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*) const;
        };


        bool negatexpr::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer) const
        {
            if ((phrase_nodes->at(index)->type == ADD1EXPR) &&
                (phrase_nodes->is_valid(index-1) &&
                 (phrase_nodes->at(index-1)->type == ident::OPERATOR) &&
                 (phrase_nodes->at(index-1)->metaValue == SUB)
                 ) &&
                !(phrase_nodes->is_valid(index-2) &&
                  (phrase_nodes->at(index-2)->type >= NEGATEXPR)
                  )
                )
            {
                phrase_t* pIndex = phrase_nodes->at(index);


                phrase_t* node =
                    new phrase_t(*pIndex, NEGATEXPR);

                node->children.add(pIndex);
                pIndex->parent = node;

                delete phrase_nodes->at(index-1);
                phrase_nodes->replace(index-1, index, node);

                return true;
            }
            else if (phrase_nodes->at(index)->type == ADD1EXPR)
            {
                setSuperType(phrase_nodes->at(index), NEGATEXPR);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // NEGATEXPR_H_INCLUDED
