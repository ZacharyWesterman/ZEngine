/**
 * File:            variable.h
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
 * Last modified:   23 Aug. 2017
**/

#pragma once
#ifndef VARIABLE_H_INCLUDED
#define VARIABLE_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        bool lexer<CHAR>::variable()
        {
            if (((phrase_nodes[index]->type == ident::IDENTIFIER) &&
                     !(phrase_nodes.is_valid(index-1) &&
                       ((phrase_nodes[index-1]->type == ident::KEYWORD_SUB) ||
                        (phrase_nodes[index-1]->type == ident::KEYWORD_TYPE) ||
                        (phrase_nodes[index-1]->type == ident::KEYWORD_VAR) ||
                        (phrase_nodes[index-1]->type == ident::IDENTIFIER))) &&
                     !(phrase_nodes.is_valid(index+1) &&
                       ((phrase_nodes[index+1]->type == ident::LBRACKET) ||
                       (phrase_nodes[index+1]->type == ident::LBRACE) ||
                       (phrase_nodes[index+1]->type == ident::LPARENTH) ||
                       (phrase_nodes[index+1]->type == ident::IDENTIFIER))) &&
                     !(phrase_nodes.is_valid(index+2) &&
                       (phrase_nodes[index+1]->type == ident::PERIOD) &&
                       (phrase_nodes[index+2]->type == ident::KEYWORD_DIM)) ))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::VARIABLE;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index]);

                node->file = phrase_nodes[index]->file;

                phrase_nodes[index] = node;

                return true;
            }
            else if ((phrase_nodes[index]->type == phrase::TYPEVAR) ||
                     ((phrase_nodes[index]->type == phrase::VARINDEX) &&
                      !(phrase_nodes.is_valid(index+1) &&
                       (phrase_nodes[index+1]->type == ident::LBRACKET))))
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::VARIABLE;

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // VARIABLE_H_INCLUDED
