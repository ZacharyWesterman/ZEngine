/**
 * File:            while_pre_stmt.h
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
#ifndef WHILE_PRE_STMT_H_INCLUDED
#define WHILE_PRE_STMT_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class while_pre_stmt : public syntaxRule
        {
        public:
            ~while_pre_stmt() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*);
        };

        bool while_pre_stmt::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer)
        {
            if (phrase_nodes->is_valid(index+5) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD) &&
                (phrase_nodes->at(index)->metaValue == LOOP) &&
                (phrase_nodes->at(index+1)->type == ident::KEYWORD) &&
                (phrase_nodes->at(index+1)->metaValue == WHILE) &&
                (phrase_nodes->at(index+2)->type == ident::LPARENTH) &&
                (phrase_nodes->at(index+3)->type == BOOLEXPR) &&
                (phrase_nodes->at(index+4)->type == ident::RPARENTH)
                )
            {
                if (phrase_nodes->is_valid(index+7) &&
                    (phrase_nodes->at(index+5)->type == ident::LBRACE) &&
                    ((phrase_nodes->at(index+6)->type == STATEMENT) ||
                     (phrase_nodes->at(index+6)->type == STATEMENTLIST)
                     ) &&
                    (phrase_nodes->at(index+7)->type == ident::RBRACE)
                    )
                {
                    phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), WHILE_PRE_STMT);

                    phrase_nodes->at(index+3)->parent = node;
                    phrase_nodes->at(index+6)->parent = node;

                    node->children.add(phrase_nodes->at(index+3));
                    node->children.add(phrase_nodes->at(index+6));

                    delete phrase_nodes->at(index);
                    delete phrase_nodes->at(index+1);
                    delete phrase_nodes->at(index+2);
                    delete phrase_nodes->at(index+4);
                    delete phrase_nodes->at(index+5);
                    delete phrase_nodes->at(index+7);
                    phrase_nodes->replace(index, index+7, node);

                    return true;
                }
                else if (phrase_nodes->is_valid(index+6) &&
                         (phrase_nodes->at(index+5)->type == ident::LBRACE) &&
                         (phrase_nodes->at(index+6)->type == ident::RBRACE)
                         )
                {
                    phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), WHILE_PRE_STMT);

                    phrase_nodes->at(index+3)->parent = node;

                    node->children.add(phrase_nodes->at(index+3));

                    delete phrase_nodes->at(index);
                    delete phrase_nodes->at(index+1);
                    delete phrase_nodes->at(index+2);
                    delete phrase_nodes->at(index+4);
                    delete phrase_nodes->at(index+5);
                    delete phrase_nodes->at(index+6);
                    phrase_nodes->replace(index, index+6, node);

                    return true;
                }
                else if (phrase_nodes->at(index+5)->type == STATEMENT)
                {
                    phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), WHILE_PRE_STMT);

                    phrase_nodes->at(index+3)->parent = node;
                    phrase_nodes->at(index+5)->parent = node;

                    node->children.add(phrase_nodes->at(index+3));
                    node->children.add(phrase_nodes->at(index+5));

                    delete phrase_nodes->at(index);
                    delete phrase_nodes->at(index+1);
                    delete phrase_nodes->at(index+2);
                    delete phrase_nodes->at(index+4);
                    phrase_nodes->replace(index, index+5, node);

                    return true;
                }
                else if (phrase_nodes->at(index+5)->type == ident::SEMICOLON)
                {
                    phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), WHILE_PRE_STMT);

                    phrase_nodes->at(index+3)->parent = node;

                    node->children.add(phrase_nodes->at(index+3));

                    delete phrase_nodes->at(index);
                    delete phrase_nodes->at(index+1);
                    delete phrase_nodes->at(index+2);
                    delete phrase_nodes->at(index+4);
                    delete phrase_nodes->at(index+5);
                    phrase_nodes->replace(index, index+5, node);

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // WHILE_PRE_STMT_H_INCLUDED
