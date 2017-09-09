/**
 * File:            powerexpr.h
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
#ifndef POWEREXPR_H_INCLUDED
#define POWEREXPR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        bool lexer<CHAR>::powerexpr()
        {
            //if no detected power operators, continue to the next phase
            if ((phrase_nodes[index]->type == phrase::NEGATEXPR) &&
                !(phrase_nodes.is_valid(index+1) &&
                  (phrase_nodes[index+1]->type == ident::OPER_POW)) &&
                !(phrase_nodes.is_valid(index-1) &&
                  (phrase_nodes[index-1]->type == ident::OPER_POW)))
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::POWEREXPR;

                return true;
            }
            //otherwise, if a power operator is detected
            else if (phrase_nodes.is_valid(index+2) &&
                     ((phrase_nodes[index]->type == phrase::NEGATEXPR) ||
                      (phrase_nodes[index]->type == phrase::POWEREXPR)) &&
                     (phrase_nodes[index+1]->type == ident::OPER_POW) &&
                     (phrase_nodes[index+2]->type == phrase::NEGATEXPR))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::POWEREXPR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+2]);

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

#endif // POWEREXPR_H_INCLUDED
