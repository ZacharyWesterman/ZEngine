/**
 * File:            dimensionexpr.h
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
#ifndef DIMENSIONEXPR_H_INCLUDED
#define DIMENSIONEXPR_H_INCLUDED

namespace z
{
    namespace script
    {
        template <typename CHAR>
        bool lexer<CHAR>::dimensionexpr()
        {
            if (phrase_nodes.is_valid(index-1) &&
                (phrase_nodes[index-1]->type == ident::KEYWORD_DIM) &&
                (phrase_nodes[index]->type == phrase::BOOLEXPR))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::DIMENSIONEXPR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index-1];
                phrase_nodes.replace(index-1, index, node);

                return true;
            }
            else
                return false;
        }
    }
}

#endif // DIMENSIONEXPR_H_INCLUDED
