/**
 * File:            typevar_decl.h
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
#ifndef TYPEVAR_DECL_H_INCLUDED
#define TYPEVAR_DECL_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class typevar_decl : public syntaxRule
        {
        public:
            ~typevar_decl() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*);
        };

        bool typevar_decl::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer)
        {
            if (phrase_nodes->is_valid(index+2) &&
                (phrase_nodes->at(index)->type == ident::IDENTIFIER) &&
                (phrase_nodes->at(index+1)->type == ident::IDENTIFIER) &&
                (phrase_nodes->at(index+2)->type == ident::SEMICOLON)
                )
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), TYPEVAR_DECL);

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+1)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+1));

                delete phrase_nodes->at(index+2);
                phrase_nodes->replace(index, index+2, node);

                return true;
            }
            else if (phrase_nodes->is_valid(index+4) &&
                     (phrase_nodes->at(index)->type == ident::IDENTIFIER) &&
                     (phrase_nodes->at(index+1)->type == ident::IDENTIFIER) &&
                     (phrase_nodes->at(index+2)->type == ident::OPERATOR) &&
                     (phrase_nodes->at(index+2)->metaValue == ASSIGN) &&
                     (phrase_nodes->at(index+3)->type == BOOLEXPR) &&
                     (phrase_nodes->at(index+4)->type == ident::SEMICOLON)
                     )
                    /*(phrase_nodes->is_valid(index+1) &&
                     phrase_nodes->is_valid(index-3) &&
                     (phrase_nodes->at(index-3)->type == ident::IDENTIFIER) &&
                     (phrase_nodes->at(index-2)->type == ident::IDENTIFIER) &&
                     (phrase_nodes->at(index-1)->type == ident::OPER_ASSIGN) &&
                     (phrase_nodes->at(index)->type == LIST) &&
                     (phrase_nodes->at(index+1)->type == ident::SEMICOLON)
                     )*/
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), TYPEVAR_DECL);

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+1)->parent = node;
                phrase_nodes->at(index+3)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+1));
                node->children.add(phrase_nodes->at(index+3));


                delete phrase_nodes->at(index+2);
                delete phrase_nodes->at(index+4);
                phrase_nodes->replace(index, index+4, node);

                return true;
            }

            return false;
        }
    }
}
}

#endif // TYPEVAR_DECL_H_INCLUDED
