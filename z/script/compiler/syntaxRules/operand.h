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
        template <typename CHAR>
        bool lexer<CHAR>::operand()
        {
            if (!(phrase_nodes.is_valid(index-1) &&
                   ((phrase_nodes[index-1]->type == ident::KEYWORD_EACH) ||
                    (phrase_nodes[index-1]->type == ident::KEYWORD_IN))))
            {
                if (((phrase_nodes[index]->type == phrase::VARIABLE) &&
                     !(phrase_nodes.is_valid(index-1) &&
                       (phrase_nodes[index-1]->type == ident::PERIOD)) &&
                     (!(phrase_nodes.is_valid(index+1) &&
                       (phrase_nodes[index+1]->type >= ident::OPER_ASSIGN) &&
                       (phrase_nodes[index+1]->type <= ident::OPER_MOD_ASSIGN)) ||
                      (phrase_nodes.is_valid(index-1) &&
                       (phrase_nodes[index-1]->type >= ident::OPER_ASSIGN) &&
                       (phrase_nodes[index-1]->type <= ident::OPER_LT_EQ))) ) ||
                    (phrase_nodes[index]->type == ident::LITERAL) ||
                    (phrase_nodes[index]->type == phrase::DIMENSIONEXPR) ||
                    (phrase_nodes[index]->type == phrase::SIZEOFEXPR) ||
                    ((phrase_nodes[index]->type == phrase::LIST) &&
                      !(phrase_nodes.is_valid(index-1) &&
                       ((phrase_nodes[index-1]->type == phrase::IDENTIFIERLIST) ||
                        (phrase_nodes[index-1]->type == ident::IDENTIFIER)))) ||
                    (phrase_nodes[index]->type == phrase::TYPE_FUNCCALL) ||
                    ((phrase_nodes[index]->type == phrase::FUNCCALL) &&
                     !(phrase_nodes.is_valid(index-1) &&
                       (phrase_nodes[index-1]->type == ident::PERIOD))))
                {
                    if (phrase_nodes[index]->orig_type == ident::NONE)
                        phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                    phrase_nodes[index]->type = phrase::OPERAND;

                    return true;
                }
            }

            return false;
        }
    }
}

#endif // OPERAND_H_INCLUDED
