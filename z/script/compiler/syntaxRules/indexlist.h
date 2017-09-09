/**
 * File:            indexlist.h
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
#ifndef INDEXLIST_H_INCLUDED
#define INDEXLIST_H_INCLUDED

namespace z
{
    namespace script
    {
        template <typename CHAR>
        bool lexer<CHAR>::indexlist()
        {
            if (phrase_nodes.is_valid(index-1) &&
                ((phrase_nodes[index-1]->type == phrase::INDEX) ||
                 (phrase_nodes[index-1]->type == phrase::INDEXLIST)) &&
                (phrase_nodes[index]->type == phrase::INDEX))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::INDEXLIST;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index-1]->parent = node;
                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index-1]);
                node->children.add(phrase_nodes[index]);

                node->file = phrase_nodes[index-1]->file;

                phrase_nodes.replace(index-1, index, node);

                return true;
            }
            else
                return false;
        }
    }
}

#endif // INDEXLIST_H_INCLUDED
