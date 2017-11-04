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

        bool lexer::return_statement()
        {
            if (phrase_nodes->is_valid(index+2) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD_RETURN) &&
                (phrase_nodes->at(index+1)->type == BOOLEXPR) &&
                (phrase_nodes->at(index+2)->type == ident::SEMICOLON))
            {
                phrase_t* node = new phrase_t();

                node->type = RETURN_STATEMENT;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                phrase_nodes->at(index+1)->parent = node;

                node->children.add(phrase_nodes->at(index+1));

                node->file = phrase_nodes->at(index)->file;

                delete phrase_nodes->at(index);
                delete phrase_nodes->at(index+2);
                phrase_nodes->replace(index, index+2, node);

                return true;
            }
            else if (phrase_nodes->is_valid(index+1) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD_RETURN) &&
                (phrase_nodes->at(index+1)->type == ident::SEMICOLON))
            {
                phrase_t* node = new phrase_t();

                node->type = RETURN_STATEMENT;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                node->file = phrase_nodes->at(index)->file;

                delete phrase_nodes->at(index);
                delete phrase_nodes->at(index+1);
                phrase_nodes->replace(index, index+1, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // RETURN_STATEMENT_H_INCLUDED
