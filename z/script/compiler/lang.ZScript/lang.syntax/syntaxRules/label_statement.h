/**
 * File:            label_statement.h
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
#ifndef LABEL_STATEMENT_H_INCLUDED
#define LABEL_STATEMENT_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        bool lexer::label_statement()
        {
            if (phrase_nodes->is_valid(index+2) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD_LABEL) &&
                (phrase_nodes->at(index+1)->type == ident::IDENTIFIER) &&
                (phrase_nodes->at(index+2)->type == ident::SEMICOLON))
            {
                phrase_t* node = new phrase_t();

                node->type = LABEL_STATEMENT;

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
            else
                return false;
        }
    }
}
}

#endif // LABEL_STATEMENT_H_INCLUDED
