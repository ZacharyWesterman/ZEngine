/**
 * File:            addexpr.h
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
 * Last modified:   19 Jul. 2017
**/

#pragma once
#ifndef ADDEXPR_H_INCLUDED
#define ADDEXPR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        bool lexer<CHAR>::addexpr()
        {
            //if no detected addition(or mul.) operators, continue to the next phase
            if ((phrase_nodes[index]->type == phrase::MULTIPLYEXPR) &&
                !(phrase_nodes.is_valid(index+1) &&
                  (phrase_nodes[index+1]->type >= ident::OPER_ADD) &&
                  (phrase_nodes[index+1]->type <= ident::OPER_MOD)) &&
                !(phrase_nodes.is_valid(index-1) &&
                  (phrase_nodes[index-1]->type >= ident::OPER_ADD) &&
                  (phrase_nodes[index-1]->type <= ident::OPER_MOD)))
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::ADDEXPR;

                return true;
            }
             //otherwise, if an addition operator is detected
            else if (phrase_nodes.is_valid(index+2) &&
                     ((phrase_nodes[index]->type == phrase::MULTIPLYEXPR) ||
                      (phrase_nodes[index]->type == phrase::ADDEXPR)) &&
                     ((phrase_nodes[index+1]->type == ident::OPER_ADD) ||
                      (phrase_nodes[index+1]->type == ident::OPER_SUB)) &&
                     ((phrase_nodes[index+2]->type == phrase::MULTIPLYEXPR) ||
                      (phrase_nodes[index+2]->type == phrase::ADDEXPR)))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::ADDEXPR;

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

#endif // ADDEXPR_H_INCLUDED
