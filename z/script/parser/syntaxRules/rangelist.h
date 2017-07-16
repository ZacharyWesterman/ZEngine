/**
 * File:            rangelist.h
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
#ifndef RANGELIST_H_INCLUDED
#define RANGELIST_H_INCLUDED

namespace z
{
    namespace script
    {
        template <typename CHAR>
        bool lexer<CHAR>::rangelist()
        {
            if (phrase_nodes.is_valid(index+2) &&
                phrase_nodes.is_valid(index-1) &&
                (phrase_nodes[index-1]->type == ident::LBRACKET) &&
                (phrase_nodes[index]->type == phrase::RANGELIST) &&
                ((phrase_nodes[index+1]->type == phrase::RANGE) ||
                 (phrase_nodes[index+1]->type == phrase::BOOLEXPR)) &&
                ((phrase_nodes[index+2]->type == ident::SEMICOLON) ||
                 (phrase_nodes[index+2]->type == ident::RBRACKET)))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::RANGELIST;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+1]);

                node->file = phrase_nodes[index]->file;

                if (phrase_nodes[index+2]->type == ident::SEMICOLON)
                {
                    delete phrase_nodes[index+2];
                    phrase_nodes.replace(index, index+2, node);
                }
                else
                {
                    phrase_nodes.replace(index, index+1, node);
                }

                return true;
            }
            else if (phrase_nodes.is_valid(index+1) &&
                phrase_nodes.is_valid(index-1) &&
                (phrase_nodes[index-1]->type == ident::LBRACKET) &&
                ((phrase_nodes[index]->type == phrase::RANGE) ||
                 (phrase_nodes[index]->type == phrase::BOOLEXPR)) &&
                (phrase_nodes[index+1]->type == ident::SEMICOLON))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::RANGELIST;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index]);

                node->file = phrase_nodes[index]->file;

                delete phrase_nodes[index+1];
                phrase_nodes.replace(index, index+1, node);

                return true;
            }
            else
                return false;
        }
    }
}

#endif // RANGELIST_H_INCLUDED
