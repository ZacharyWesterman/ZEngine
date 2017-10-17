/**
 * File:            factorialexpr.h
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
#ifndef FACTORIALEXPR_H_INCLUDED
#define FACTORIALEXPR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        class factorialexpr : public syntaxRule<CHAR>
        {
        public:
            ~factorialexpr() {}

            bool apply(core::array< phrase_t<CHAR>* >*,
                       int);
        };

        template <typename CHAR>
        bool factorialexpr<CHAR>::apply(core::array< phrase_t<CHAR>* >* phrase_nodes,
                                  int index)
        {
            if ((phrase_nodes->at(index)->type == PARENTHEXPR) &&
                (phrase_nodes->is_valid(index+1) &&
                 (phrase_nodes->at(index+1)->type == ident::OPERATOR) &&
                 (phrase_nodes->at(index+1)->metaValue == FAC)
                 )
                )
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = FACTORIALEXPR;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                node->children.add(phrase_nodes->at(index));

                phrase_nodes->at(index)->parent = node;

                node->file = phrase_nodes->at(index)->file;

                delete phrase_nodes->at(index+1);
                phrase_nodes->replace(index, index+1, node);

                return true;
            }
            else if ((phrase_nodes->at(index)->type == PARENTHEXPR) &&
                     !(phrase_nodes->is_valid(index+1) &&
                       (phrase_nodes->at(index+1)->type == ident::PERIOD)
                       )
                     ) //prioritize type-vars over expressions
            {
                if (phrase_nodes->at(index)->orig_type == ident::NONE)
                    phrase_nodes->at(index)->orig_type = phrase_nodes->at(index)->type;

                phrase_nodes->at(index)->type = FACTORIALEXPR;

                return true;
            }

            return false;
        }
    }
}
}

#endif // FACTORIALEXPR_H_INCLUDED
