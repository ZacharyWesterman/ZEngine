/**
 * File:            type_funccall.h
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
 * Last modified:   18 Aug. 2017
**/

#pragma once
#ifndef TYPE_FUNCCALL_H_INCLUDED
#define TYPE_FUNCCALL_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class type_funccall : public syntaxRule
        {
        public:
            ~type_funccall() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*) const;
        };

        bool type_funccall::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer) const
        {
            if (phrase_nodes->is_valid(index+2) &&
                (phrase_nodes->at(index)->type == PARENTHEXPR) &&
                (phrase_nodes->at(index+1)->type == ident::PERIOD) &&
                (phrase_nodes->at(index+2)->type == FUNCCALL)
                )
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), TYPE_FUNCCALL);

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

#endif // TYPE_FUNCCALL_H_INCLUDED
