/**
 * File:            boolexpr.h
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
 * Last modified:   21 Jul. 2017
**/

#pragma once
#ifndef BOOLEXPR_H_INCLUDED
#define BOOLEXPR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        bool lexer<CHAR>::boolexpr()
        {
            //if no detected boolean(or add.) operators, continue to the next phase
            if ((phrase_nodes[index]->type == phrase::ASSIGNEXPR) ||
                ((phrase_nodes[index]->type == phrase::ADDEXPR) &&
                !(phrase_nodes.is_valid(index+1) &&
                  (phrase_nodes[index+1]->type >= ident::OPER_ADD) &&
                  (phrase_nodes[index+1]->type <= ident::OPER_LT_EQ)) &&
                !(phrase_nodes.is_valid(index-1) &&
                  (phrase_nodes[index-1]->type >= ident::OPER_ADD) &&
                  (phrase_nodes[index-1]->type <= ident::OPER_LT_EQ))))
            {
                if (phrase_nodes[index]->orig_type == ident::NONE)
                    phrase_nodes[index]->orig_type = phrase_nodes[index]->type;
                phrase_nodes[index]->type = phrase::BOOLEXPR;

                return true;
            }
            //otherwise, if a binary boolean operator is detected
            else if (phrase_nodes.is_valid(index+2) &&
                     ((phrase_nodes[index]->type == phrase::ADDEXPR) ||
                      (phrase_nodes[index]->type == phrase::BOOLEXPR)) &&
                     (phrase_nodes[index+1]->type >= ident::OPER_AND_LGCL) &&
                     (phrase_nodes[index+1]->type <= ident::OPER_LT_EQ) &&
                     (phrase_nodes[index+2]->type == phrase::ADDEXPR))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::BOOLEXPR;

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
            //or a unary boolean operator
            else if (phrase_nodes.is_valid(index+1) &&
                     ((phrase_nodes[index+1]->type == phrase::ADDEXPR) ||
                      (phrase_nodes[index+1]->type == phrase::BOOLEXPR)) &&
                     ((phrase_nodes[index]->type == ident::OPER_NOT_LGCL) ||
                      (phrase_nodes[index]->type == ident::OPER_NOT_BITW)))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::BOOLEXPR;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index+1]->parent = node;
                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index+1]);
                node->children.add(phrase_nodes[index]);

                node->file = phrase_nodes[index]->file;

                phrase_nodes.replace(index, index+1, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // BOOLEXPR_H_INCLUDED
