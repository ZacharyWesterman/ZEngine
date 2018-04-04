/**
 * File:            until_statement.h
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
#ifndef UNTIL_STATEMENT_H_INCLUDED
#define UNTIL_STATEMENT_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class until_statement : public syntaxRule
        {
        public:
            ~until_statement() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*);
        };

        bool until_statement::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer)
        {
            if (phrase_nodes->is_valid(index+3) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD) &&
                (phrase_nodes->at(index)->metaValue == WAIT) &&
                (phrase_nodes->at(index+1)->type == ident::KEYWORD) &&
                (phrase_nodes->at(index+1)->metaValue == UNTIL) &&
                (phrase_nodes->at(index+2)->type == BOOLEXPR) &&
                (phrase_nodes->at(index+3)->type == ident::SEMICOLON)
                )
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), UNTIL_STATEMENT);

                phrase_nodes->at(index+2)->parent = node;

                node->children.add(phrase_nodes->at(index+2));

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

#endif // UNTIL_STATEMENT_H_INCLUDED
