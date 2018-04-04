/**
 * File:            return_statement.h
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
#ifndef RETURN_STATEMENT_H_INCLUDED
#define RETURN_STATEMENT_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class return_statement : public syntaxRule
        {
        public:
            ~return_statement() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*);
        };


        bool return_statement::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer)
        {
            if (phrase_nodes->is_valid(index+2) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD) &&
                (phrase_nodes->at(index)->metaValue == RETURN) &&
                (phrase_nodes->at(index+1)->type == BOOLEXPR) &&
                (phrase_nodes->at(index+2)->type == ident::SEMICOLON)
                )
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), RETURN_STATEMENT);

                phrase_nodes->at(index+1)->parent = node;

                node->children.add(phrase_nodes->at(index+1));

                delete phrase_nodes->at(index);
                delete phrase_nodes->at(index+2);
                phrase_nodes->replace(index, index+2, node);

                return true;
            }
            else if (phrase_nodes->is_valid(index+1) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD) &&
                (phrase_nodes->at(index)->metaValue == RETURN) &&
                (phrase_nodes->at(index+1)->type == ident::SEMICOLON))
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), RETURN_STATEMENT);

                delete phrase_nodes->at(index);
                delete phrase_nodes->at(index+1);
                phrase_nodes->replace(index, index+1, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // RETURN_STATEMENT_H_INCLUDED
