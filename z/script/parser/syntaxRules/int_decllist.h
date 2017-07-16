/**
 * File:            int_decllist.h
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
#ifndef INT_DECLLIST_H_INCLUDED
#define INT_DECLLIST_H_INCLUDED

namespace z
{
    namespace script
    {
        template <typename CHAR>
        bool lexer<CHAR>::int_decllist()
        {
            if (((phrase_nodes.is_valid(index-4) &&
                 (phrase_nodes[index-4]->type == ident::KEYWORD_TYPE) &&
                 (phrase_nodes[index-3]->type == ident::IDENTIFIER) &&
                 (phrase_nodes[index-2]->type == ident::LBRACE) &&
                 ((phrase_nodes[index-1]->type == phrase::VARIABLE_DECL) ||
                  (phrase_nodes[index-1]->type == phrase::TYPEVAR_DECL) ||
                  (phrase_nodes[index-1]->type == phrase::FUNCTION_DECL))) ||

                 (phrase_nodes.is_valid(index-1) &&
                  (phrase_nodes[index-1]->type == phrase::INT_DECLLIST))) &&

                ((phrase_nodes[index]->type == phrase::VARIABLE_DECL) ||
                 (phrase_nodes[index]->type == phrase::TYPEVAR_DECL) ||
                 (phrase_nodes[index]->type == phrase::FUNCTION_DECL)))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::INT_DECLLIST;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index-1]->parent = node;
                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index-1]);
                node->children.add(phrase_nodes[index]);

                node->file = phrase_nodes[index-1]->file;

                phrase_nodes.replace(index-1, index, node);

                return true;
            }
            else
                return false;
        }
    }
}

#endif // INT_DECLLIST_H_INCLUDED
