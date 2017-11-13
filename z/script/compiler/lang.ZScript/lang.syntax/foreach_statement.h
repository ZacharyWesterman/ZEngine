/**
 * File:            foreach_statement.h
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
#ifndef FOREACH_STATEMENT_H_INCLUDED
#define FOREACH_STATEMENT_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class foreach_statement : public syntaxRule
        {
        public:
            ~foreach_statement() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*) const;
        };

        bool foreach_statement::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer) const
        {
            if (phrase_nodes->is_valid(index+6) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD) &&
                (phrase_nodes->at(index)->metaValue == FOR) &&
                (phrase_nodes->at(index+1)->type == ident::KEYWORD) &&
                (phrase_nodes->at(index+1)->metaValue == EACH) &&
                (phrase_nodes->at(index+2)->type == ident::LPARENTH) &&
                ((phrase_nodes->at(index+3)->type == VARIABLE_DECL) ||
                 (phrase_nodes->at(index+3)->type == TYPEVAR_DECL)
                 ) &&
                (phrase_nodes->at(index+4)->type == BOOLEXPR) &&
                (phrase_nodes->at(index+5)->type == ident::RPARENTH)
                )
            {
                if (phrase_nodes->at(index+6)->type == ident::SEMICOLON)
                {
                    phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), FOREACH_STATEMENT);

                    phrase_nodes->at(index+3)->parent = node;
                    phrase_nodes->at(index+4)->parent = node;

                    node->children.add(phrase_nodes->at(index+3));
                    node->children.add(phrase_nodes->at(index+4));

                    delete phrase_nodes->at(index);
                    delete phrase_nodes->at(index+1);
                    delete phrase_nodes->at(index+2);
                    delete phrase_nodes->at(index+5);
                    delete phrase_nodes->at(index+6);
                    phrase_nodes->replace(index, index+6, node);

                    return true;
                }
                else if (phrase_nodes->at(index+6)->type == STATEMENT)
                {
                    phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), FOREACH_STATEMENT);

                    phrase_nodes->at(index+3)->parent = node;
                    phrase_nodes->at(index+4)->parent = node;
                    phrase_nodes->at(index+6)->parent = node;

                    node->children.add(phrase_nodes->at(index+3));
                    node->children.add(phrase_nodes->at(index+4));
                    node->children.add(phrase_nodes->at(index+6));


                    delete phrase_nodes->at(index);
                    delete phrase_nodes->at(index+1);
                    delete phrase_nodes->at(index+2);
                    delete phrase_nodes->at(index+5);
                    phrase_nodes->replace(index, index+6, node);

                    return true;
                }
                else if (phrase_nodes->is_valid(index+7) &&
                         (phrase_nodes->at(index+6)->type == ident::LBRACE) &&
                         (phrase_nodes->at(index+7)->type == ident::RBRACE)
                         )
                {
                    phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), FOREACH_STATEMENT);

                    phrase_nodes->at(index+3)->parent = node;
                    phrase_nodes->at(index+4)->parent = node;

                    node->children.add(phrase_nodes->at(index+3));
                    node->children.add(phrase_nodes->at(index+4));


                    delete phrase_nodes->at(index);
                    delete phrase_nodes->at(index+1);
                    delete phrase_nodes->at(index+2);
                    delete phrase_nodes->at(index+5);
                    delete phrase_nodes->at(index+6);
                    delete phrase_nodes->at(index+7);
                    phrase_nodes->replace(index, index+7, node);

                    return true;
                }
                else if (phrase_nodes->is_valid(index+8) &&
                         (phrase_nodes->at(index+6)->type == ident::LBRACE) &&
                         ((phrase_nodes->at(index+7)->type == STATEMENT) ||
                          (phrase_nodes->at(index+7)->type == STATEMENTLIST)
                          ) &&
                         (phrase_nodes->at(index+8)->type == ident::RBRACE)
                         )
                {
                    phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), FOREACH_STATEMENT);

                    phrase_nodes->at(index+3)->parent = node;
                    phrase_nodes->at(index+4)->parent = node;
                    phrase_nodes->at(index+7)->parent = node;

                    node->children.add(phrase_nodes->at(index+3));
                    node->children.add(phrase_nodes->at(index+4));
                    node->children.add(phrase_nodes->at(index+7));


                    delete phrase_nodes->at(index);
                    delete phrase_nodes->at(index+1);
                    delete phrase_nodes->at(index+2);
                    delete phrase_nodes->at(index+5);
                    delete phrase_nodes->at(index+6);
                    delete phrase_nodes->at(index+8);
                    phrase_nodes->replace(index, index+8, node);

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // FOREACH_STATEMENT_H_INCLUDED
