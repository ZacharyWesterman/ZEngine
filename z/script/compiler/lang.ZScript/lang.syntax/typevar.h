/**
 * File:            typevar.h
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
#ifndef TYPEVAR_H_INCLUDED
#define TYPEVAR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class typevar : public syntaxRule
        {
        public:
            ~typevar() {}

            bool apply(core::array< phrase_t* >*,
                       int);
        };

        bool typevar::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index)
        {
            if (phrase_nodes->is_valid(index+2) &&
                (phrase_nodes->at(index)->type == PARENTHEXPR) &&
                (phrase_nodes->at(index+1)->type == ident::PERIOD) &&
                (phrase_nodes->at(index+2)->type == VARIABLE))
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), TYPEVAR);

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+2)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+2));

                delete phrase_nodes->at(index+1);
                phrase_nodes->replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // TYPEVAR_H_INCLUDED
