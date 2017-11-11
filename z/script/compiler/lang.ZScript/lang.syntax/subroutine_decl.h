/**
 * File:            subroutine_decl.h
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
#ifndef SUBROUTINE_DECL_H_INCLUDED
#define SUBROUTINE_DECL_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class subroutine_decl : public syntaxRule
        {
        public:
            ~subroutine_decl() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*);
        };

        bool subroutine_decl::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer)
        {
            if (phrase_nodes->is_valid(index+5) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD) &&
                (phrase_nodes->at(index)->metaValue == SUBR) &&
                (phrase_nodes->at(index+1)->type == ident::IDENTIFIER) &&
                (phrase_nodes->at(index+2)->type == ident::LBRACE) &&
                ((phrase_nodes->at(index+3)->type == STATEMENT) ||
                 (phrase_nodes->at(index+3)->type == STATEMENTLIST)
                 ) &&
                (phrase_nodes->at(index+4)->type == ident::RBRACE) &&
                (phrase_nodes->at(index+5)->type == ident::SEMICOLON)
                )
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), SUBROUTINE_DECL);

                phrase_nodes->at(index+1)->parent = node;
                phrase_nodes->at(index+3)->parent = node;

                node->children.add(phrase_nodes->at(index+1));
                node->children.add(phrase_nodes->at(index+3));

                delete phrase_nodes->at(index);
                delete phrase_nodes->at(index+2);
                delete phrase_nodes->at(index+4);
                phrase_nodes->replace(index, index+4, node);

                return true;
            }
            else if (phrase_nodes->is_valid(index+4) &&
                     (phrase_nodes->at(index)->type == ident::KEYWORD) &&
                     (phrase_nodes->at(index)->metaValue == SUBR) &&
                     (phrase_nodes->at(index+1)->type == ident::IDENTIFIER) &&
                     (phrase_nodes->at(index+2)->type == ident::LBRACE) &&
                     (phrase_nodes->at(index+3)->type == ident::RBRACE) &&
                     (phrase_nodes->at(index+4)->type == ident::SEMICOLON)
                     )
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), SUBROUTINE_DECL);

                phrase_nodes->at(index+1)->parent = node;

                node->children.add(phrase_nodes->at(index+1));

                delete phrase_nodes->at(index);
                delete phrase_nodes->at(index+2);
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

#endif // SUBROUTINE_DECL_H_INCLUDED
