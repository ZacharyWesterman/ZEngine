/**
 * File:            range.h
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
#ifndef RANGE_H_INCLUDED
#define RANGE_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        bool lexer<CHAR>::_range()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == phrase::BOOLEXPR) &&
                ((phrase_nodes[index+1]->type == ident::OPER_R_ARROW) ||
                 (phrase_nodes[index+1]->type == ident::OPER_L_ARROW)) &&
                (phrase_nodes[index+2]->type == phrase::BOOLEXPR))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::RANGE;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+2]->parent = node;

                if (phrase_nodes[index+1]->type == ident::OPER_R_ARROW)
                {
                    node->children.add(phrase_nodes[index]);
                    node->children.add(phrase_nodes[index+2]);
                }
                else
                {
                    node->children.add(phrase_nodes[index+2]);
                    node->children.add(phrase_nodes[index]);
                }

                node->file = phrase_nodes[index]->file;

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

#endif // RANGE_H_INCLUDED
