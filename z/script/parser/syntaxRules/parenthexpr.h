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
 * Last modified:   15 Jul. 2017
**/

#pragma once
#ifndef PARENTHEXPR_H_INCLUDED
#define PARENTHEXPR_H_INCLUDED

namespace z
{
    namespace script
    {
        template <typename CHAR>
        bool lexer<CHAR>::parenthexpr()
        {

            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::LPARENTH) &&
                (phrase_nodes[index+2]->type == ident::RPARENTH) &&
                (phrase_nodes[index+1]->type == phrase::BOOLEXPR) &&
                     !(phrase_nodes.is_valid(index-1) &&
                       ((phrase_nodes[index-1]->type == ident::IDENTIFIER) ||
                        (phrase_nodes[index-1]->type == ident::KEYWORD_IF) ||
                        (phrase_nodes[index-1]->type == ident::KEYWORD_WHILE))))
            {
                if (phrase_nodes[index+1]->orig_type == ident::NONE)
                    phrase_nodes[index+1]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index+1]->type = phrase::PARENTHEXPR;

                delete phrase_nodes[index];
                delete phrase_nodes[index+2];
                phrase_nodes.remove(index+2);
                phrase_nodes.remove(index);

                return true;
            }
            else if ((phrase_nodes[index]->type == phrase::OPERAND) ||
                     (phrase_nodes[index]->type == phrase::SIZEOFEXPR))
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::PARENTHEXPR;

                return true;
            }
            else
                return false;
        }
    }
}

#endif // PARENTHEXPR_H_INCLUDED
