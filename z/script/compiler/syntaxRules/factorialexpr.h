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
        template <typename CHAR>
        bool lexer<CHAR>::factorialexpr()
        {
            if ((phrase_nodes[index]->type == phrase::PARENTHEXPR) &&
                (phrase_nodes.is_valid(index+1) &&
                      (phrase_nodes[index+1]->type == ident::OPER_FAC)))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::FACTORIALEXPR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                node->children.add(phrase_nodes[index]);

                phrase_nodes[index]->parent = node;

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index+1];
                phrase_nodes.replace(index, index+1, node);

                return true;
            }
            else if ((phrase_nodes[index]->type == phrase::PARENTHEXPR) &&
                     !(phrase_nodes.is_valid(index+1) &&
                       (phrase_nodes[index+1]->type == ident::PERIOD))) //prioritize type-vars over expressions
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::FACTORIALEXPR;

                return true;
            }
            else
                return false;
        }
    }
}

#endif // FACTORIALEXPR_H_INCLUDED
