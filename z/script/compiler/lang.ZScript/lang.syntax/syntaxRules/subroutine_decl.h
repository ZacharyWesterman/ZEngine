/**
 * File:            subroutine_decl.h
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
#ifndef SUBROUTINE_DECL_H_INCLUDED
#define SUBROUTINE_DECL_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        bool lexer::subroutine_decl()
        {
            if (phrase_nodes.is_valid(index+4) &&
                (phrase_nodes[index]->type == ident::KEYWORD_SUB) &&
                (phrase_nodes[index+1]->type == ident::IDENTIFIER) &&
                (phrase_nodes[index+2]->type == ident::LBRACE) &&
                ((phrase_nodes[index+3]->type == phrase::STATEMENT) ||
                 (phrase_nodes[index+3]->type == phrase::STATEMENTLIST)) &&
                (phrase_nodes[index+4]->type == ident::RBRACE))
            {
                phrase_t* node = new phrase_t();

                node->type = phrase::SUBROUTINE_DECL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+1]->parent = node;
                phrase_nodes[index+3]->parent = node;

                node->children.add(phrase_nodes[index+1]);
                node->children.add(phrase_nodes[index+3]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+2];
                delete phrase_nodes[index+4];
                phrase_nodes.replace(index, index+4, node);

                return true;
            }
            else if (phrase_nodes.is_valid(index+3) &&
                     (phrase_nodes[index]->type == ident::KEYWORD_SUB) &&
                     (phrase_nodes[index+1]->type == ident::IDENTIFIER) &&
                     (phrase_nodes[index+2]->type == ident::LBRACE) &&
                     (phrase_nodes[index+3]->type == ident::RBRACE))
            {
                phrase_t* node = new phrase_t();

                node->type = phrase::SUBROUTINE_DECL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index+1]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index];
                delete phrase_nodes[index+2];
                delete phrase_nodes[index+3];
                phrase_nodes.replace(index, index+3, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // SUBROUTINE_DECL_H_INCLUDED
