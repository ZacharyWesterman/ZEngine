/**
 * File:            variable_decl.h
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
 * Last modified:   29 Aug. 2017
**/

#pragma once
#ifndef VARIABLE_DECL_H_INCLUDED
#define VARIABLE_DECL_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        bool lexer::variable_decl()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::KEYWORD_VAR) &&
                (phrase_nodes[index+1]->type == ident::IDENTIFIER))
            {
                if (phrase_nodes[index+2]->type == ident::SEMICOLON)
                {
                    phrase_t* node = new phrase_t();

                    node->type = phrase::VARIABLE_DECL;

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
                else if (phrase_nodes.is_valid(index+4) &&
                         (phrase_nodes[index+2]->type == ident::OPER_ASSIGN) &&
                         (phrase_nodes[index+3]->type == phrase::BOOLEXPR) &&
                         (phrase_nodes[index+4]->type == ident::SEMICOLON))
                {
                    //variable declaration
                    phrase_t* idCpy = new phrase_t(*phrase_nodes[index+1]);

                    phrase_t* node = new phrase_t();

                    node->type = phrase::VARIABLE_DECL;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    idCpy->parent = node;

                    node->children.add(idCpy);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];

                    phrase_nodes[index] = node;

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // VARIABLE_DECL_H_INCLUDED
