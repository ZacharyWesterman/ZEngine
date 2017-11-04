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

        bool lexer::for_statement()
        {
            if (phrase_nodes.is_valid(index+7) &&
                (phrase_nodes[index]->type == ident::KEYWORD_FOR) &&
                (phrase_nodes[index+1]->type == ident::LPARENTH) &&
                (phrase_nodes[index+2]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+3]->type == ident::SEMICOLON) &&
                (phrase_nodes[index+4]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+5]->type == ident::SEMICOLON) &&
                (phrase_nodes[index+6]->type == phrase::BOOLEXPR) &&
                ((phrase_nodes[index+7]->type == ident::RPARENTH) ||
                 (phrase_nodes.is_valid(index+8) &&
                  (phrase_nodes[index+7]->type == ident::SEMICOLON) &&
                  (phrase_nodes[index+8]->type == ident::RPARENTH))))
            {
                if (phrase_nodes[index+7]->type == ident::SEMICOLON)
                {
                    delete phrase_nodes[index+7];
                    phrase_nodes.remove(index+7);
                }

                if (phrase_nodes.is_valid(index+10) &&
                    (phrase_nodes[index+8]->type == ident::LBRACE) &&
                    ((phrase_nodes[index+9]->type == phrase::STATEMENT) ||
                     (phrase_nodes[index+9]->type == phrase::STATEMENTLIST)) &&
                    (phrase_nodes[index+10]->type == ident::RBRACE))
                {
                    phrase_t* node = new phrase_t();

                    node->type = phrase::FOR_STATEMENT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+2]->parent = node;
                    phrase_nodes[index+4]->parent = node;
                    phrase_nodes[index+6]->parent = node;
                    phrase_nodes[index+9]->parent = node;

                    node->children.add(phrase_nodes[index+2]);
                    node->children.add(phrase_nodes[index+4]);
                    node->children.add(phrase_nodes[index+6]);
                    node->children.add(phrase_nodes[index+9]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+3];
                    delete phrase_nodes[index+5];
                    delete phrase_nodes[index+7];
                    delete phrase_nodes[index+8];
                    delete phrase_nodes[index+10];
                    phrase_nodes.replace(index, index+10, node);

                    return true;
                }
                else if (phrase_nodes.is_valid(index+9) &&
                    (phrase_nodes[index+8]->type == ident::LBRACE) &&
                    (phrase_nodes[index+9]->type == ident::RBRACE))
                {
                    phrase_t* node = new phrase_t();

                    node->type = phrase::FOR_STATEMENT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+2]->parent = node;
                    phrase_nodes[index+4]->parent = node;
                    phrase_nodes[index+6]->parent = node;

                    node->children.add(phrase_nodes[index+2]);
                    node->children.add(phrase_nodes[index+4]);
                    node->children.add(phrase_nodes[index+6]);



                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+3];
                    delete phrase_nodes[index+5];
                    delete phrase_nodes[index+7];
                    delete phrase_nodes[index+8];
                    delete phrase_nodes[index+9];
                    phrase_nodes.replace(index, index+9, node);

                    return true;
                }
                if (phrase_nodes.is_valid(index+8))
                {
                    if (phrase_nodes[index+8]->type == phrase::STATEMENT)
                    {
                        phrase_t* node = new phrase_t();

                        node->type = phrase::FOR_STATEMENT;

                        node->line = phrase_nodes[index]->line;
                        node->column = phrase_nodes[index]->column;

                        phrase_nodes[index+2]->parent = node;
                        phrase_nodes[index+4]->parent = node;
                        phrase_nodes[index+6]->parent = node;
                        phrase_nodes[index+8]->parent = node;

                        node->children.add(phrase_nodes[index+2]);
                        node->children.add(phrase_nodes[index+4]);
                        node->children.add(phrase_nodes[index+6]);
                        node->children.add(phrase_nodes[index+8]);



                        delete phrase_nodes[index];
                        delete phrase_nodes[index+1];
                        delete phrase_nodes[index+3];
                        delete phrase_nodes[index+5];
                        delete phrase_nodes[index+7];
                        phrase_nodes.replace(index, index+8, node);

                        return true;
                    }
                    else if (phrase_nodes[index+8]->type == ident::SEMICOLON)
                    {
                        phrase_t* node = new phrase_t();

                        node->type = phrase::FOR_STATEMENT;

                        node->line = phrase_nodes[index]->line;
                        node->column = phrase_nodes[index]->column;

                        phrase_nodes[index+2]->parent = node;
                        phrase_nodes[index+4]->parent = node;
                        phrase_nodes[index+6]->parent = node;

                        node->children.add(phrase_nodes[index+2]);
                        node->children.add(phrase_nodes[index+4]);
                        node->children.add(phrase_nodes[index+6]);



                        delete phrase_nodes[index];
                        delete phrase_nodes[index+1];
                        delete phrase_nodes[index+3];
                        delete phrase_nodes[index+5];
                        delete phrase_nodes[index+7];
                        delete phrase_nodes[index+8];
                        phrase_nodes.replace(index, index+8, node);

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
