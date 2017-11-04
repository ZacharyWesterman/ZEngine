/**
 * File:            loop_statement.h
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
#ifndef LOOP_STATEMENT_H_INCLUDED
#define LOOP_STATEMENT_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        bool lexer::loop_statement()
        {
            if (phrase_nodes->is_valid(index+3) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD_LOOP) &&
                (phrase_nodes->at(index+1)->type == ident::LBRACE) &&
                ((phrase_nodes->at(index+2)->type == STATEMENT) ||
                 (phrase_nodes->at(index+2)->type == STATEMENTLIST)) &&
                (phrase_nodes->at(index+3)->type == ident::RBRACE) &&
                !(phrase_nodes->is_valid(index+4) &&
                  (phrase_nodes->at(index+4)->type == ident::KEYWORD_WHILE)))
            {
                phrase_t* node = new phrase_t();

                node->type = LOOP_STATEMENT;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                phrase_nodes->at(index+2)->parent = node;

                node->children.add(phrase_nodes->at(index+2));

                node->file = phrase_nodes->at(index)->file;

                delete phrase_nodes->at(index);
                delete phrase_nodes->at(index+1);
                delete phrase_nodes->at(index+3);
                phrase_nodes->replace(index, index+3, node);

                return true;

            }
            else if (phrase_nodes->is_valid(index+2) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD_LOOP) &&
                (phrase_nodes->at(index+1)->type == ident::LBRACE) &&
                (phrase_nodes->at(index+2)->type == ident::RBRACE) &&
                !(phrase_nodes->is_valid(index+3) &&
                  (phrase_nodes->at(index+3)->type == ident::KEYWORD_WHILE)))
            {
                phrase_t* node = new phrase_t();

                node->type = LOOP_STATEMENT;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;



                delete phrase_nodes->at(index);
                delete phrase_nodes->at(index+1);
                delete phrase_nodes->at(index+2);
                phrase_nodes->replace(index, index+2, node);

                return true;

            }
            else if (phrase_nodes->is_valid(index+1) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD_LOOP) &&
                !(phrase_nodes->is_valid(index+2) &&
                  (phrase_nodes->at(index+2)->type == ident::KEYWORD_WHILE)))
            {
                if (phrase_nodes->at(index+1)->type == ident::SEMICOLON)
                {
                    phrase_t* node = new phrase_t();

                    node->type = LOOP_STATEMENT;

                    node->line = phrase_nodes->at(index)->line;
                    node->column = phrase_nodes->at(index)->column;



                    delete phrase_nodes->at(index);
                    delete phrase_nodes->at(index+1);
                    phrase_nodes->replace(index, index+1, node);

                    return true;
                }
                else if (phrase_nodes->at(index+1)->type == STATEMENT)
                {
                    phrase_t* node = new phrase_t();

                    node->type = LOOP_STATEMENT;

                    node->line = phrase_nodes->at(index)->line;
                    node->column = phrase_nodes->at(index)->column;

                    phrase_nodes->at(index+1)->parent = node;

                    node->children.add(phrase_nodes->at(index+1));



                    delete phrase_nodes->at(index);
                    phrase_nodes->replace(index, index+1, node);

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // LOOP_STATEMENT_H_INCLUDED
