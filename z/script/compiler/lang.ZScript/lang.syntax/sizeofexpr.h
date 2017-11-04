/**
 * File:            sizeofexpr.h
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
#ifndef SIZEOFEXPR_H_INCLUDED
#define SIZEOFEXPR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        class sizeofexpr : public syntaxRule
        {
        public:
            ~sizeofexpr() {}

            bool apply(core::array< phrase_t* >*,
                       int);
        };


        bool sizeofexpr::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index)
        {
            if (phrase_nodes->is_valid(index+3) &&
                (phrase_nodes->at(index)->type == ident::OPERATOR) &&
                (phrase_nodes->at(index)->metaValue == SIZEOF) &&
                (phrase_nodes->at(index+1)->type == ident::LPARENTH) &&
                (phrase_nodes->at(index+2)->type == BOOLEXPR) &&
                (phrase_nodes->at(index+3)->type == ident::RPARENTH)
                )
            {
                phrase_t* node = new phrase_t();

                node->type = SIZEOFEXPR;

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
            else
                return false;
        }
    }
}
}

#endif // SIZEOFEXPR_H_INCLUDED
