/**
 * File:            parenthexpr.h
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
#ifndef PARENTHEXPR_H_INCLUDED
#define PARENTHEXPR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        class parenthexpr : public syntaxRule<CHAR>
        {
        public:
            ~parenthexpr() {}

            bool apply(core::array< phrase_t<CHAR>* >*,
                       int);
        };

        template <typename CHAR>
        bool parenthexpr<CHAR>::apply(core::array< phrase_t<CHAR>* >* phrase_nodes,
                                  int index)
        {

            if (phrase_nodes->is_valid(index+2) &&
                (phrase_nodes->at(index)->type == ident::LPARENTH) &&
                (phrase_nodes->at(index+2)->type == ident::RPARENTH) &&
                (phrase_nodes->at(index+1)->type == BOOLEXPR) &&
                !(phrase_nodes->is_valid(index-1) &&
                  ((phrase_nodes->at(index-1)->type == ident::IDENTIFIER) ||
                   (phrase_nodes->at(index-1)->type == ident::KEYWORD)
                   )
                  )
                )
            {
                setSuperType(phrase_nodes->at(index+1), PARENTHEXPR);

                delete phrase_nodes->at(index);
                delete phrase_nodes->at(index+2);
                phrase_nodes->remove(index+2);
                phrase_nodes->remove(index);

                return true;
            }
            else if ((phrase_nodes->at(index)->type == OPERAND) &&
                     !(phrase_nodes->is_valid(index-1) &&
                       (phrase_nodes->at(index-1)->type == ident::LPARENTH)
                       )
                     )
            {
                setSuperType(phrase_nodes->at(index), PARENTHEXPR);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // PARENTHEXPR_H_INCLUDED
