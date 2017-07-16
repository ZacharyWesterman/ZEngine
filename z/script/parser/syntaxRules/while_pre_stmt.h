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
        template <typename CHAR>
        bool lexer<CHAR>::while_pre_stmt()
        {
            if (phrase_nodes.is_valid(index+5) &&
                (phrase_nodes[index]->type == ident::KEYWORD_LOOP) &&
                (phrase_nodes[index+1]->type == ident::KEYWORD_WHILE) &&
                (phrase_nodes[index+2]->type == ident::LPARENTH) &&
                (phrase_nodes[index+3]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+4]->type == ident::RPARENTH))
            {
                if (phrase_nodes.is_valid(index+7) &&
                    (phrase_nodes[index+5]->type == ident::LBRACE) &&
                    ((phrase_nodes[index+6]->type == phrase::STATEMENT) ||
                     (phrase_nodes[index+6]->type == phrase::STATEMENTLIST)) &&
                    (phrase_nodes[index+7]->type == ident::RBRACE))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::WHILE_PRE_STMT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+3]->parent = node;
                    phrase_nodes[index+6]->parent = node;

                    node->children.add(phrase_nodes[index+3]);
                    node->children.add(phrase_nodes[index+6]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+2];
                    delete phrase_nodes[index+4];
                    delete phrase_nodes[index+5];
                    delete phrase_nodes[index+7];
                    phrase_nodes.replace(index, index+7, node);

                    return true;
                }
                else if (phrase_nodes.is_valid(index+6) &&
                    (phrase_nodes[index+5]->type == ident::LBRACE) &&
                    (phrase_nodes[index+6]->type == ident::RBRACE))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::WHILE_PRE_STMT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+3]->parent = node;

                    node->children.add(phrase_nodes[index+3]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+2];
                    delete phrase_nodes[index+4];
                    delete phrase_nodes[index+5];
                    delete phrase_nodes[index+6];
                    phrase_nodes.replace(index, index+6, node);

                    return true;
                }
                else if (phrase_nodes[index+5]->type == phrase::STATEMENT)
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::WHILE_PRE_STMT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+3]->parent = node;
                    phrase_nodes[index+5]->parent = node;

                    node->children.add(phrase_nodes[index+3]);
                    node->children.add(phrase_nodes[index+5]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+2];
                    delete phrase_nodes[index+4];
                    phrase_nodes.replace(index, index+5, node);

                    return true;
                }
                else if (phrase_nodes[index+5]->type == ident::SEMICOLON)
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::WHILE_PRE_STMT;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+3]->parent = node;

                    node->children.add(phrase_nodes[index+3]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    delete phrase_nodes[index+2];
                    delete phrase_nodes[index+4];
                    delete phrase_nodes[index+5];
                    phrase_nodes.replace(index, index+5, node);

                    return true;
                }
            }

            return false;
        }
    }
}

#endif // WHILE_PRE_STMT_H_INCLUDED
