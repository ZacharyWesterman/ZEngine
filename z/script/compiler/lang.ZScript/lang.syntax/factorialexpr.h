/**
 * File:            factorialexpr.h
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
 * Last modified:   18 Aug. 2017
**/

#pragma once
#ifndef FACTORIALEXPR_H_INCLUDED
#define FACTORIALEXPR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        class factorialexpr : public syntaxRule<CHAR>
        {
        public:
            ~factorialexpr() {}

            bool apply(core::array< phrase_t<CHAR>* >*,
                       int);
        };

        template <typename CHAR>
        bool factorialexpr<CHAR>::apply(core::array< phrase_t<CHAR>* >* phrase_nodes,
                                  int index)
        {
            if ((phrase_nodes->at(index)->type == PARENTHEXPR) &&
                (phrase_nodes->is_valid(index+1) &&
                 (phrase_nodes->at(index+1)->type == ident::OPERATOR) &&
                 (phrase_nodes->at(index+1)->metaValue == FAC)
                 )
                )
            {
                phrase_t<CHAR>* pIndex = phrase_nodes->at(index);

                phrase_t<CHAR>* node =
                    new phrase_t<CHAR>(*pIndex, FACTORIALEXPR);

                node->children.add(pIndex);
                pIndex->parent = node;

                delete phrase_nodes->at(index+1);
                phrase_nodes->replace(index, index+1, node);

                return true;
            }
            else if ((phrase_nodes->at(index)->type == PARENTHEXPR) &&
                     !(phrase_nodes->is_valid(index+1) &&
                       (phrase_nodes->at(index+1)->type == ident::PERIOD)
                       )
                     ) //prioritize type-vars over expressions
            {
                setSuperType(phrase_nodes->at(index), FACTORIALEXPR);

                return true;
            }

            return false;
        }
    }
}
}

#endif // FACTORIALEXPR_H_INCLUDED
