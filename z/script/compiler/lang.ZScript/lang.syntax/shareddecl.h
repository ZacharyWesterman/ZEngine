/**
 * File:            shareddecl.h
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
#ifndef SHAREDDECL_H_INCLUDED
#define SHAREDDECL_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class shareddecl : public syntaxRule
        {
        public:
            ~shareddecl() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*);
        };

        bool shareddecl::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer)
        {
            if (phrase_nodes->is_valid(index+1) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD) &&
                (phrase_nodes->at(index)->metaValue == SHARED) &&
                ((phrase_nodes->at(index+1)->type == VARIABLE_DECL) ||
                 (phrase_nodes->at(index+1)->type == TYPEVAR_DECL)
                 )
                )
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), SHAREDDECL);

                phrase_nodes->at(index+1)->parent = node;

                node->children.add(phrase_nodes->at(index+1));

                delete phrase_nodes->at(index);
                phrase_nodes->replace(index, index+1, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // SHAREDDECL_H_INCLUDED
