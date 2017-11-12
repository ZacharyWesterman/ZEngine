/**
 * File:            for_statement.h
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
#ifndef FOR_STATEMENT_H_INCLUDED
#define FOR_STATEMENT_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class for_statement : public syntaxRule
        {
        public:
            ~for_statement() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*);
        };


        bool for_statement::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer)
        {
            if (phrase_nodes->is_valid(index+7) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD) &&
                (phrase_nodes->at(index)->metaValue == FOR) &&
                (phrase_nodes->at(index+1)->type == ident::LPARENTH) &&
                (phrase_nodes->at(index+2)->type == BOOLEXPR) &&
                (phrase_nodes->at(index+3)->type == ident::SEMICOLON) &&
                (phrase_nodes->at(index+4)->type == BOOLEXPR) &&
                (phrase_nodes->at(index+5)->type == ident::SEMICOLON) &&
                (phrase_nodes->at(index+6)->type == BOOLEXPR) &&
                ((phrase_nodes->at(index+7)->type == ident::RPARENTH) ||
                 (phrase_nodes->is_valid(index+8) &&
                  (phrase_nodes->at(index+7)->type == ident::SEMICOLON) &&
                  (phrase_nodes->at(index+8)->type == ident::RPARENTH))))
            {
                if (phrase_nodes->at(index+7)->type == ident::SEMICOLON)
                {
                    delete phrase_nodes->at(index+7);
                    phrase_nodes->remove(index+7);
                }

                //for ( ... ) { ... }
                if (phrase_nodes->is_valid(index+10) &&
                    (phrase_nodes->at(index+8)->type == ident::LBRACE) &&
                    ((phrase_nodes->at(index+9)->type == STATEMENT) ||
                     (phrase_nodes->at(index+9)->type == STATEMENTLIST)) &&
                    (phrase_nodes->at(index+10)->type == ident::RBRACE))
                {
                    phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), FOR_STATEMENT);

                    phrase_nodes->at(index+2)->parent = node;
                    phrase_nodes->at(index+4)->parent = node;
                    phrase_nodes->at(index+6)->parent = node;
                    phrase_nodes->at(index+9)->parent = node;

                    node->children.add(phrase_nodes->at(index+2));
                    node->children.add(phrase_nodes->at(index+4));
                    node->children.add(phrase_nodes->at(index+6));
                    node->children.add(phrase_nodes->at(index+9));

                    delete phrase_nodes->at(index);
                    delete phrase_nodes->at(index+1);
                    delete phrase_nodes->at(index+3);
                    delete phrase_nodes->at(index+5);
                    delete phrase_nodes->at(index+7);
                    delete phrase_nodes->at(index+8);
                    delete phrase_nodes->at(index+10);
                    phrase_nodes->replace(index, index+10, node);

                    return true;
                }
                //for ( ... ) { }
                else if (phrase_nodes->is_valid(index+9) &&
                    (phrase_nodes->at(index+8)->type == ident::LBRACE) &&
                    (phrase_nodes->at(index+9)->type == ident::RBRACE))
                {
                    phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), FOR_STATEMENT);

                    phrase_nodes->at(index+2)->parent = node;
                    phrase_nodes->at(index+4)->parent = node;
                    phrase_nodes->at(index+6)->parent = node;

                    node->children.add(phrase_nodes->at(index+2));
                    node->children.add(phrase_nodes->at(index+4));
                    node->children.add(phrase_nodes->at(index+6));



                    delete phrase_nodes->at(index);
                    delete phrase_nodes->at(index+1);
                    delete phrase_nodes->at(index+3);
                    delete phrase_nodes->at(index+5);
                    delete phrase_nodes->at(index+7);
                    delete phrase_nodes->at(index+8);
                    delete phrase_nodes->at(index+9);
                    phrase_nodes->replace(index, index+9, node);

                    return true;
                }
                if (phrase_nodes->is_valid(index+8))
                {
                    //for ( ... ) STATEMENT
                    if (phrase_nodes->at(index+8)->type == STATEMENT)
                    {
                        phrase_t* node =
                        new phrase_t(*(phrase_nodes->at(index)), FOR_STATEMENT);

                        phrase_nodes->at(index+2)->parent = node;
                        phrase_nodes->at(index+4)->parent = node;
                        phrase_nodes->at(index+6)->parent = node;
                        phrase_nodes->at(index+8)->parent = node;

                        node->children.add(phrase_nodes->at(index+2));
                        node->children.add(phrase_nodes->at(index+4));
                        node->children.add(phrase_nodes->at(index+6));
                        node->children.add(phrase_nodes->at(index+8));



                        delete phrase_nodes->at(index);
                        delete phrase_nodes->at(index+1);
                        delete phrase_nodes->at(index+3);
                        delete phrase_nodes->at(index+5);
                        delete phrase_nodes->at(index+7);
                        phrase_nodes->replace(index, index+8, node);

                        return true;
                    }
                    //for ( ... ) ;
                    else if (phrase_nodes->at(index+8)->type == ident::SEMICOLON)
                    {
                        phrase_t* node =
                        new phrase_t(*(phrase_nodes->at(index)), FOR_STATEMENT);

                        phrase_nodes->at(index+2)->parent = node;
                        phrase_nodes->at(index+4)->parent = node;
                        phrase_nodes->at(index+6)->parent = node;

                        node->children.add(phrase_nodes->at(index+2));
                        node->children.add(phrase_nodes->at(index+4));
                        node->children.add(phrase_nodes->at(index+6));



                        delete phrase_nodes->at(index);
                        delete phrase_nodes->at(index+1);
                        delete phrase_nodes->at(index+3);
                        delete phrase_nodes->at(index+5);
                        delete phrase_nodes->at(index+7);
                        delete phrase_nodes->at(index+8);
                        phrase_nodes->replace(index, index+8, node);

                        return true;
                    }
                }
            }

            return false;
        }
    }
}
}

#endif // FOR_STATEMENT_H_INCLUDED
