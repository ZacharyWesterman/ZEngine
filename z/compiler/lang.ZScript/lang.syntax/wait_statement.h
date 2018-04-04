/**
 * File:            wait_statement.h
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
#ifndef WAIT_STATEMENT_H_INCLUDED
#define WAIT_STATEMENT_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class wait_statement : public syntaxRule
        {
        public:
            ~wait_statement() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*);
        };

        bool wait_statement::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer)
        {
            if (phrase_nodes->is_valid(index+2) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD) &&
                (phrase_nodes->at(index)->metaValue == WAIT) &&
                (phrase_nodes->at(index+1)->type == BOOLEXPR) &&
                (phrase_nodes->at(index+2)->type == ident::SEMICOLON)
                )
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), WAIT_STATEMENT);

                phrase_nodes->at(index+1)->parent = node;

                node->children.add(phrase_nodes->at(index+1));

                delete phrase_nodes->at(index);
                delete phrase_nodes->at(index+2);
                phrase_nodes->replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // WAIT_STATEMENT_H_INCLUDED
