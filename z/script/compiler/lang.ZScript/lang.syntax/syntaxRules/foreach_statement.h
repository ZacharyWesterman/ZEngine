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

        bool lexer::foreach_statement()
        {
            if (phrase_nodes.is_valid(index+5) &&
                (phrase_nodes[index]->type == ident::KEYWORD_FOR) &&
                (phrase_nodes[index+1]->type == ident::KEYWORD_EACH) &&
                (phrase_nodes[index+2]->type == phrase::VARIABLE) &&
                (phrase_nodes[index+3]->type == ident::KEYWORD_IN) &&
                ((phrase_nodes[index+4]->type == phrase::VARIABLE) ||
                 (phrase_nodes[index+4]->type == phrase::LIST)))
            {
                if (phrase_nodes[index+5]->type == ident::SEMICOLON)
                {
                    phrase_t* node = new phrase_t();

                    node->type = phrase::FOREACH_STATEMENT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+2]->parent = node;
                    phrase_nodes[index+4]->parent = node;

                    node->children.add(phrase_nodes[index+2]);
                    node->children.add(phrase_nodes[index+4]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+3];
                    delete phrase_nodes[index+5];
                    phrase_nodes.replace(index, index+5, node);

                    return true;
                }
                else if (phrase_nodes[index+5]->type == phrase::STATEMENT)
                {
                    phrase_t* node = new phrase_t();

                    node->type = phrase::FOREACH_STATEMENT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+2]->parent = node;
                    phrase_nodes[index+4]->parent = node;
                    phrase_nodes[index+5]->parent = node;

                    node->children.add(phrase_nodes[index+2]);
                    node->children.add(phrase_nodes[index+4]);
                    node->children.add(phrase_nodes[index+5]);



                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+3];
                    phrase_nodes.replace(index, index+5, node);

                    return true;
                }
                else if (phrase_nodes.is_valid(index+6) &&
                         (phrase_nodes[index+5]->type == ident::LBRACE) &&
                         (phrase_nodes[index+6]->type == ident::RBRACE))
                {
                    phrase_t* node = new phrase_t();

                    node->type = phrase::FOREACH_STATEMENT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+2]->parent = node;
                    phrase_nodes[index+4]->parent = node;

                    node->children.add(phrase_nodes[index+2]);
                    node->children.add(phrase_nodes[index+4]);



                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+3];
                    delete phrase_nodes[index+5];
                    delete phrase_nodes[index+6];
                    phrase_nodes.replace(index, index+6, node);

                    return true;
                }
                else if (phrase_nodes.is_valid(index+7) &&
                         (phrase_nodes[index+5]->type == ident::LBRACE) &&
                         ((phrase_nodes[index+6]->type == phrase::STATEMENT) ||
                          (phrase_nodes[index+6]->type == phrase::STATEMENTLIST)) &&
                         (phrase_nodes[index+7]->type == ident::RBRACE))
                {
                    phrase_t* node = new phrase_t();

                    node->type = phrase::FOREACH_STATEMENT;

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
                    phrase_nodes.replace(index, index+7, node);

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // FOREACH_STATEMENT_H_INCLUDED
