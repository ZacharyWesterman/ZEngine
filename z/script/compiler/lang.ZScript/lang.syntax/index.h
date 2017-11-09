/**
 * File:            index.h
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
#ifndef INDEX_H_INCLUDED
#define INDEX_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class _index : public syntaxRule
        {
        public:
            ~_index() {}

            bool apply(core::array< phrase_t* >*,
                       int);
        };


        bool _index::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index)
        {
            if (phrase_nodes->is_valid(index+2) &&
                (phrase_nodes->at(index)->type == ident::LBRACKET) &&
                ((phrase_nodes->at(index+1)->type == BOOLEXPR) ||
                 (phrase_nodes->at(index+1)->type == RANGE) ||
                 (phrase_nodes->at(index+1)->type == RANGELIST)
                 ) &&
                (phrase_nodes->at(index+2)->type == ident::RBRACKET)
                )
            {
                setSuperType(phrase_nodes->at(index+1), INDEX);

                delete phrase_nodes->at(index);
                delete phrase_nodes->at(index+2);

                phrase_nodes->remove(index+2);
                phrase_nodes->remove(index);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // INDEX_H_INCLUDED
