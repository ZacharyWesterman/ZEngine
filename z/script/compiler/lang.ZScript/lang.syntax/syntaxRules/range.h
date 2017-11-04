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

        bool lexer::_range()
        {
            if (phrase_nodes->is_valid(index+2) &&
                (phrase_nodes->at(index)->type == BOOLEXPR) &&
                ((phrase_nodes->at(index+1)->type == ident::OPER_R_ARROW) ||
                 (phrase_nodes->at(index+1)->type == ident::OPER_L_ARROW)) &&
                (phrase_nodes->at(index+2)->type == BOOLEXPR))
            {
                phrase_t* node = new phrase_t();

                node->type = RANGE;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+2)->parent = node;

                if (phrase_nodes->at(index+1)->type == ident::OPER_R_ARROW)
                {
                    node->children.add(phrase_nodes->at(index));
                    node->children.add(phrase_nodes->at(index+2));
                }
                else
                {
                    node->children.add(phrase_nodes->at(index+2));
                    node->children.add(phrase_nodes->at(index));
                }

                node->file = phrase_nodes->at(index)->file;

                delete phrase_nodes->at(index+1);
                phrase_nodes->replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // RANGE_H_INCLUDED
