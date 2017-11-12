/**
 * File:            variable_decl.h
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
 * Last modified:   29 Aug. 2017
**/

#pragma once
#ifndef VARIABLE_DECL_H_INCLUDED
#define VARIABLE_DECL_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class variable_decl : public syntaxRule
        {
        public:
            ~variable_decl() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*);
        };

        bool variable_decl::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer)
        {
            if (phrase_nodes->is_valid(index+2) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD) &&
                (phrase_nodes->at(index)->metaValue == VAR) &&
                (phrase_nodes->at(index+1)->type == IDENTIFIER)
                )
            {
                if (phrase_nodes->at(index+2)->type == ident::SEMICOLON)
                {
                    phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), VARIABLE_DECL);

                    phrase_nodes->at(index+1)->parent = node;

                    node->children.add(phrase_nodes->at(index+1));

                    delete phrase_nodes->at(index);
                    delete phrase_nodes->at(index+2);
                    phrase_nodes->replace(index, index+2, node);

                    return true;
                }
                else if (phrase_nodes->is_valid(index+4) &&
                         (phrase_nodes->at(index+2)->type == ident::OPERATOR) &&
                         (phrase_nodes->at(index+2)->metaValue == ASSIGN) &&
                         (phrase_nodes->at(index+3)->type == BOOLEXPR) &&
                         (phrase_nodes->at(index+4)->type == ident::SEMICOLON)
                         )
                {
                    phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), VARIABLE_DECL);

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
            }

            return false;
        }
    }
}
}

#endif // VARIABLE_DECL_H_INCLUDED
