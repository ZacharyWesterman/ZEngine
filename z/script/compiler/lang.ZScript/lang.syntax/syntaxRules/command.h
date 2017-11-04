/**
 * File:            command.h
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
 * Last modified:   2 Aug. 2017
**/

#pragma once
#ifndef _COMMAND_H_INCLUDED
#define _COMMAND_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        bool lexer::_command()
        {
            if (phrase_nodes.is_valid(index+3) &&
                (phrase_nodes[index]->type == phrase::IDENTIFIERLIST) &&
                (phrase_nodes[index+1]->type == ident::LBRACE) &&
                ((phrase_nodes[index+2]->type == phrase::EXPRLIST) ||
                 (phrase_nodes[index+2]->type == phrase::BOOLEXPR)) &&
                (phrase_nodes[index+3]->type == ident::RBRACE))
            {
                phrase_t* node = new phrase_t();

                node->type = phrase::COMMAND;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+2]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index+1];
                delete phrase_nodes[index+3];
                phrase_nodes.replace(index, index+3, node);

                return true;
            }
            else if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == phrase::IDENTIFIERLIST) &&
                (phrase_nodes[index+1]->type == ident::LBRACE) &&
                (phrase_nodes[index+2]->type == ident::RBRACE))
            {
                phrase_t* node = new phrase_t();

                node->type = phrase::COMMAND;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index+1];
                delete phrase_nodes[index+2];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // _COMMAND_H_INCLUDED
