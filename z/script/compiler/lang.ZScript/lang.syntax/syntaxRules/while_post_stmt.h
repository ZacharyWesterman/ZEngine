/**
 * File:            while_post_stmt.h
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
#ifndef WHILE_POST_STMT_H_INCLUDED
#define WHILE_POST_STMT_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        bool lexer::while_post_stmt()
        {
            if (phrase_nodes->is_valid(index+7) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD_LOOP) &&
                (phrase_nodes->at(index+1)->type == ident::LBRACE) &&
                ((phrase_nodes->at(index+2)->type == STATEMENT) ||
                 (phrase_nodes->at(index+2)->type == STATEMENTLIST)) &&
                (phrase_nodes->at(index+3)->type == ident::RBRACE) &&
                (phrase_nodes->at(index+4)->type == ident::KEYWORD_WHILE) &&
                (phrase_nodes->at(index+5)->type == ident::LPARENTH) &&
                (phrase_nodes->at(index+6)->type == BOOLEXPR) &&
                (phrase_nodes->at(index+7)->type == ident::RPARENTH))
            {
                phrase_t* node = new phrase_t();

                node->type = WHILE_POST_STMT;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                phrase_nodes->at(index+2)->parent = node;
                phrase_nodes->at(index+6)->parent = node;

                node->children.add(phrase_nodes->at(index+2));
                node->children.add(phrase_nodes->at(index+6));

                node->file = phrase_nodes->at(index)->file;

                delete phrase_nodes->at(index);
                delete phrase_nodes->at(index+1);
                delete phrase_nodes->at(index+3);
                delete phrase_nodes->at(index+4);
                delete phrase_nodes->at(index+5);
                delete phrase_nodes->at(index+7);
                phrase_nodes->replace(index, index+7, node);

                return true;

            }
            else if (phrase_nodes->is_valid(index+6) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD_LOOP) &&
                (phrase_nodes->at(index+1)->type == ident::LBRACE) &&
                (phrase_nodes->at(index+2)->type == ident::RBRACE) &&
                (phrase_nodes->at(index+3)->type == ident::KEYWORD_WHILE) &&
                (phrase_nodes->at(index+4)->type == ident::LPARENTH) &&
                (phrase_nodes->at(index+5)->type == BOOLEXPR) &&
                (phrase_nodes->at(index+6)->type == ident::RPARENTH))
            {
                phrase_t* node = new phrase_t();

                node->type = WHILE_POST_STMT;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                phrase_nodes->at(index+5)->parent = node;

                node->children.add(phrase_nodes->at(index+5));

                node->file = phrase_nodes->at(index)->file;

                delete phrase_nodes->at(index);
                delete phrase_nodes->at(index+1);
                delete phrase_nodes->at(index+2);
                delete phrase_nodes->at(index+3);
                delete phrase_nodes->at(index+4);
                delete phrase_nodes->at(index+6);
                phrase_nodes->replace(index, index+6, node);

                return true;

            }
            else if (phrase_nodes->is_valid(index+5) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD_LOOP) &&
                (phrase_nodes->at(index+2)->type == ident::KEYWORD_WHILE) &&
                (phrase_nodes->at(index+3)->type == ident::LPARENTH) &&
                (phrase_nodes->at(index+4)->type == BOOLEXPR) &&
                (phrase_nodes->at(index+5)->type == ident::RPARENTH))
            {
                if (phrase_nodes->at(index+1)->type == STATEMENT)
                {
                    phrase_t* node = new phrase_t();

                    node->type = WHILE_POST_STMT;

                    node->line = phrase_nodes->at(index)->line;
                    node->column = phrase_nodes->at(index)->column;

                    phrase_nodes->at(index+1)->parent = node;
                    phrase_nodes->at(index+4)->parent = node;

                    node->children.add(phrase_nodes->at(index+1));
                    node->children.add(phrase_nodes->at(index+4));

                    node->file = phrase_nodes->at(index)->file;

                    delete phrase_nodes->at(index);
                    delete phrase_nodes->at(index+2);
                    delete phrase_nodes->at(index+3);
                    delete phrase_nodes->at(index+5);
                    phrase_nodes->replace(index, index+5, node);

                    return true;
                }
                else if (phrase_nodes->at(index+1)->type == ident::SEMICOLON)
                {
                    phrase_t* node = new phrase_t();

                    node->type = WHILE_POST_STMT;

                    node->line = phrase_nodes->at(index)->line;
                    node->column = phrase_nodes->at(index)->column;

                    phrase_nodes->at(index+4)->parent = node;

                    node->children.add(phrase_nodes->at(index+4));


                    delete phrase_nodes->at(index);
                    delete phrase_nodes->at(index+1);
                    delete phrase_nodes->at(index+2);
                    delete phrase_nodes->at(index+3);
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

#endif // WHILE_POST_STMT_H_INCLUDED
