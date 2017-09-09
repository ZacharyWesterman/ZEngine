/**
 * File:            type_funccall.h
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
 * Last modified:   18 Aug. 2017
**/

#pragma once
#ifndef TYPE_FUNCCALL_H_INCLUDED
#define TYPE_FUNCCALL_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        bool lexer<CHAR>::type_funccall()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == phrase::PARENTHEXPR) &&
                (phrase_nodes[index+1]->type == ident::PERIOD) &&
                (phrase_nodes[index+2]->type == phrase::FUNCCALL))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::TYPE_FUNCCALL;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                node->file = phrase_nodes[index]->file;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+2]);

                delete phrase_nodes[index+1];
                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // TYPE_FUNCCALL_H_INCLUDED
