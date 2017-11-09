/**
 * File:            operand.h
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
 * Last modified:   18 Jul. 2017
**/

#pragma once
#ifndef OPERAND_H_INCLUDED
#define OPERAND_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class operand : public syntaxRule
        {
        public:
            ~operand() {}

            bool apply(core::array< phrase_t* >*,
                       int);
        };

        bool operand::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index)
        {
            if (!(phrase_nodes->is_valid(index-1) &&
                  (phrase_nodes->at(index-1)->type == ident::KEYWORD) &&
                  ((phrase_nodes->at(index-1)->metaValue == IN) ||
                   (phrase_nodes->at(index-1)->metaValue == EACH)
                   )
                  )
                )
            {
                /*if (((phrase_nodes->at(index)->type == VARIABLE) &&
                     !(phrase_nodes->is_valid(index-1) &&
                       (phrase_nodes->at(index-1)->type == ident::PERIOD)) &&
                     (!(phrase_nodes->is_valid(index+1) &&
                       (phrase_nodes->at(index+1)->type >= ident::OPERATOR)) ||
                      (phrase_nodes->is_valid(index-1) &&
                       (phrase_nodes->at(index-1)->type >= ident::OPERATOR))) ) ||

                    ((phrase_nodes->at(index)->type == ident::LITERAL) &&
                     !(phrase_nodes->is_valid(index+1) &&
                       (phrase_nodes->at(index+1)->type == ident::LBRACKET))) ||

                    (phrase_nodes->at(index)->type == DIMENSIONEXPR) ||
                    (phrase_nodes->at(index)->type == SIZEOFEXPR) ||
                    ((phrase_nodes->at(index)->type == LIST) &&
                      !(phrase_nodes->is_valid(index-1) &&
                       ((phrase_nodes->at(index-1)->type == IDENTIFIERLIST) ||
                        (phrase_nodes->at(index-1)->type == ident::IDENTIFIER))) &&
                     !(phrase_nodes->is_valid(index+1) &&
                       (phrase_nodes->at(index+1)->type == ident::LBRACKET))) ||
                    (phrase_nodes->at(index)->type == TYPE_FUNCCALL) ||
                    ((phrase_nodes->at(index)->type == FUNCCALL) &&
                     !(phrase_nodes->is_valid(index-1) &&
                       (phrase_nodes->at(index-1)->type == ident::PERIOD))))*/
                if (((phrase_nodes->at(index)->type == VARIABLE) &&
                     !(phrase_nodes->is_valid(index+1) &&
                       (phrase_nodes->at(index+1)->type == ident::OPERATOR) &&
                       (phrase_nodes->at(index+1)->metaValue >= ASSIGN) &&
                       (phrase_nodes->at(index+1)->metaValue <= MOD_ASSIGN)
                       ) &&
                     !(phrase_nodes->is_valid(index-1) &&
                       (phrase_nodes->at(index-1)->type == ident::PERIOD)
                       )
                     ) ||
                    (phrase_nodes->at(index)->type == ident::LITERAL) ||
                    (phrase_nodes->at(index)->type == VARINDEX) ||
                    (phrase_nodes->at(index)->type == DIMENSIONEXPR) ||
                    (phrase_nodes->at(index)->type == SIZEOFEXPR) ||
                    (phrase_nodes->at(index)->type == TYPE_FUNCCALL) ||
                    (phrase_nodes->at(index)->type == TYPEVAR) ||
                    ((phrase_nodes->at(index)->type == FUNCCALL) &&
                     !(phrase_nodes->is_valid(index-1) &&
                       (phrase_nodes->at(index-1)->type == ident::PERIOD)
                       )
                     ) ||
                    ((phrase_nodes->at(index)->type == LIST) &&
                     !(phrase_nodes->is_valid(index-1) &&
                       ((phrase_nodes->at(index-1)->type == IDENTIFIERLIST) ||
                        (phrase_nodes->at(index-1)->type == ident::IDENTIFIER)
                        )
                       )
                     )
                    )
                { //std::cout << phrase_nodes->at(index)->type << std::endl;
                    setSuperType(phrase_nodes->at(index),OPERAND);

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // OPERAND_H_INCLUDED
