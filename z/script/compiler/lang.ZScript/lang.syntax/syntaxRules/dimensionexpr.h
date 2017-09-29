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
 * Last modified:   22 Aug. 2017
**/

#pragma once
#ifndef DIMENSIONEXPR_H_INCLUDED
#define DIMENSIONEXPR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        bool lexer<CHAR>::dimensionexpr()
        {
            if (phrase_nodes.is_valid(index+5) &&
                (phrase_nodes[index]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+1]->type == ident::PERIOD) &&
                (phrase_nodes[index+2]->type == ident::KEYWORD_DIM) &&
                (phrase_nodes[index+3]->type == ident::LPARENTH) &&
                (phrase_nodes[index+4]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+5]->type == ident::RPARENTH))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::DIMENSIONEXPR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+4]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+4]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index+1];
                delete phrase_nodes[index+2];
                delete phrase_nodes[index+3];
                delete phrase_nodes[index+5];
                phrase_nodes.replace(index, index+5, node);

                return true;
            }
            else if (!(phrase_nodes.is_valid(index-2) &&
                       (phrase_nodes[index-1]->type == ident::PERIOD) &&
                       (phrase_nodes[index-2]->type == ident::IDENTIFIER)) &&
                     phrase_nodes.is_valid(index+3) &&
                     (phrase_nodes[index]->type == ident::KEYWORD_DIM) &&
                     (phrase_nodes[index+1]->type == ident::LPARENTH) &&
                     (phrase_nodes[index+2]->type == phrase::BOOLEXPR) &&
                     (phrase_nodes[index+3]->type == ident::RPARENTH))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::DIMENSIONEXPR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index+2]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+1];
                delete phrase_nodes[index+3];
                phrase_nodes.replace(index, index+3, node);

                return true;
            }

            return false;
        }
    }
}
}

#endif // DIMENSIONEXPR_H_INCLUDED
