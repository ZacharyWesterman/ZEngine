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

        bool lexer::dimensionexpr()
        {
            if (phrase_nodes->is_valid(index+5) &&
                (phrase_nodes->at(index)->type == ident::IDENTIFIER) &&
                (phrase_nodes->at(index+1)->type == ident::PERIOD) &&
                (phrase_nodes->at(index+2)->type == ident::KEYWORD_DIM) &&
                (phrase_nodes->at(index+3)->type == ident::LPARENTH) &&
                (phrase_nodes->at(index+4)->type == BOOLEXPR) &&
                (phrase_nodes->at(index+5)->type == ident::RPARENTH))
            {
                phrase_t* node = new phrase_t();

                node->type = DIMENSIONEXPR;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+4)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+4));

                node->file = phrase_nodes->at(index)->file;

                delete phrase_nodes->at(index+1);
                delete phrase_nodes->at(index+2);
                delete phrase_nodes->at(index+3);
                delete phrase_nodes->at(index+5);
                phrase_nodes->replace(index, index+5, node);

                return true;
            }
            else if (!(phrase_nodes->is_valid(index-2) &&
                       (phrase_nodes->at(index-1)->type == ident::PERIOD) &&
                       (phrase_nodes->at(index-2)->type == ident::IDENTIFIER)) &&
                     phrase_nodes->is_valid(index+3) &&
                     (phrase_nodes->at(index)->type == ident::KEYWORD_DIM) &&
                     (phrase_nodes->at(index+1)->type == ident::LPARENTH) &&
                     (phrase_nodes->at(index+2)->type == BOOLEXPR) &&
                     (phrase_nodes->at(index+3)->type == ident::RPARENTH))
            {
                phrase_t* node = new phrase_t();

                node->type = DIMENSIONEXPR;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                phrase_nodes->at(index+2)->parent = node;

                node->children.add(phrase_nodes->at(index+2));

                node->file = phrase_nodes->at(index)->file;

                delete phrase_nodes->at(index);
                delete phrase_nodes->at(index+1);
                delete phrase_nodes->at(index+3);
                phrase_nodes->replace(index, index+3, node);

                return true;
            }

            return false;
        }
    }
}
}

#endif // DIMENSIONEXPR_H_INCLUDED
