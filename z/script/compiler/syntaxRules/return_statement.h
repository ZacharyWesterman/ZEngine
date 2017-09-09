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
        template <typename CHAR>
        bool lexer<CHAR>::return_statement()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::KEYWORD_RETURN) &&
                (phrase_nodes[index+1]->type == phrase::BOOLEXPR) &&
                (phrase_nodes[index+2]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::RETURN_STATEMENT;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index+1]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+2];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else if (phrase_nodes.is_valid(index+1) &&
                (phrase_nodes[index]->type == ident::KEYWORD_RETURN) &&
                (phrase_nodes[index+1]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::RETURN_STATEMENT;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+1];
                phrase_nodes.replace(index, index+1, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // RETURN_STATEMENT_H_INCLUDED
