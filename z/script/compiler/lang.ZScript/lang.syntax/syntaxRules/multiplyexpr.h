/**
 * File:            multiplyexpr.h
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
#ifndef MULTIPLYEXPR_H_INCLUDED
#define MULTIPLYEXPR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        bool lexer<CHAR>::multiplyexpr()
        {
            //if no detected multiplication operators, continue to the next phase
            if ((phrase_nodes[index]->type == phrase::POWEREXPR) &&
                !(phrase_nodes.is_valid(index+1) &&
                  (phrase_nodes[index+1]->type >= ident::OPER_MUL) &&
                  (phrase_nodes[index+1]->type <= ident::OPER_MOD)) &&
                !(phrase_nodes.is_valid(index-1) &&
                  (phrase_nodes[index-1]->type >= ident::OPER_MUL) &&
                  (phrase_nodes[index-1]->type <= ident::OPER_MOD)))
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::MULTIPLYEXPR;

                return true;
            }
            //otherwise, if a multiplication operator is detected
            else if (phrase_nodes.is_valid(index+2) &&
                     ((phrase_nodes[index]->type == phrase::POWEREXPR) ||
                      (phrase_nodes[index]->type == phrase::MULTIPLYEXPR)) &&
                     (phrase_nodes[index+1]->type >= ident::OPER_MUL) &&
                     (phrase_nodes[index+1]->type <= ident::OPER_MOD) &&
                     (phrase_nodes[index+2]->type == phrase::POWEREXPR))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::MULTIPLYEXPR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+1]->parent = node;
                phrase_nodes[index+2]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+1]);
                node->children.add(phrase_nodes[index+2]);

                node->file = phrase_nodes[index]->file;

                phrase_nodes.replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // MULTIPLYEXPR_H_INCLUDED
