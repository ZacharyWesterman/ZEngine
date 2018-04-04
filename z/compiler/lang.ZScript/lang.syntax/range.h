/**
 * File:            range.h
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
#ifndef RANGE_H_INCLUDED
#define RANGE_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class _range : public syntaxRule
        {
        public:
            ~_range() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*);
        };


        bool _range::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer)
        {
            if (phrase_nodes->is_valid(index+2) &&
                (phrase_nodes->at(index)->type == BOOLEXPR) &&
                (phrase_nodes->at(index+1)->type == ident::OPERATOR) &&
                ((phrase_nodes->at(index+1)->metaValue == R_ARROW) ||
                 (phrase_nodes->at(index+1)->metaValue == L_ARROW)
                 ) &&
                (phrase_nodes->at(index+2)->type == BOOLEXPR)
                )
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), RANGE);

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+2)->parent = node;

                if (phrase_nodes->at(index+1)->metaValue == R_ARROW)
                {
                    node->children.add(phrase_nodes->at(index));
                    node->children.add(phrase_nodes->at(index+2));
                }
                else
                {
                    node->children.add(phrase_nodes->at(index+2));
                    node->children.add(phrase_nodes->at(index));
                }

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

#endif // RANGE_H_INCLUDED
