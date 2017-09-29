/**
 * File:            shareddecl.h
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
#ifndef SHAREDDECL_H_INCLUDED
#define SHAREDDECL_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        bool lexer<CHAR>::shareddecl()
        {
            if (phrase_nodes.is_valid(index+1) &&
                phrase_nodes.is_valid(index-1) &&
                (phrase_nodes[index-1]->type == ident::KEYWORD_SHARED) &&
                (phrase_nodes[index]->type == phrase::VARIABLE) &&
                (phrase_nodes[index+1]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::SHAREDDECL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index]);

                node->file = phrase_nodes[index-1]->file;

                delete phrase_nodes[index-1];
                delete phrase_nodes[index+1];
                phrase_nodes.replace(index-1, index+1, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // SHAREDDECL_H_INCLUDED
