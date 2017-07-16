/**
 * File:            until_statement.h
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
#ifndef UNTIL_STATEMENT_H_INCLUDED
#define UNTIL_STATEMENT_H_INCLUDED

namespace z
{
    namespace script
    {
        template <typename CHAR>
        bool lexer<CHAR>::until_statement()
        {
            if (phrase_nodes.is_valid(index+3) &&
                (phrase_nodes[index]->type == ident::KEYWORD_WAIT) &&
                (phrase_nodes[index+1]->type == ident::KEYWORD_UNTIL) &&
                (phrase_nodes[index+2]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+3]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::UNTIL_STATEMENT;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index+2]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+1];
                delete phrase_nodes[index+3];
                phrase_nodes.replace(index, index+3, node);

                return true;
            }
            else
                return false;
        }
    }
}

#endif // UNTIL_STATEMENT_H_INCLUDED
