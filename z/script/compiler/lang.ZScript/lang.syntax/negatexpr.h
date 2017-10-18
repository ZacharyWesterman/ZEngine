/**
 * File:            negatexpr.h
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
#ifndef NEGATEXPR_H_INCLUDED
#define NEGATEXPR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        class negatexpr : public syntaxRule<CHAR>
        {
        public:
            ~negatexpr() {}

            bool apply(core::array< phrase_t<CHAR>* >*,
                       int);
        };

        template <typename CHAR>
        bool negatexpr<CHAR>::apply(core::array< phrase_t<CHAR>* >* phrase_nodes,
                                  int index)
        {
            if ((phrase_nodes->at(index)->type == ADD1EXPR) &&
                (phrase_nodes->is_valid(index-1) &&
                 (phrase_nodes->at(index-1)->type == ident::OPERATOR) &&
                 (phrase_nodes->at(index-1)->metaValue == SUB)
                 ) &&
                !(phrase_nodes->is_valid(index-2) &&
                  (phrase_nodes->at(index-2)->type >= NEGATEXPR)
                  )
                )
            {

                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = NEGATEXPR;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                node->children.add(phrase_nodes->at(index));

                phrase_nodes->at(index)->parent = node;

                node->file = phrase_nodes->at(index)->file;

                delete phrase_nodes->at(index-1);
                phrase_nodes->replace(index-1, index, node);

                return true;
            }
            else if (phrase_nodes->at(index)->type == ADD1EXPR)
            {
                if (phrase_nodes->at(index)->orig_type == ident::NONE)
                    phrase_nodes->at(index)->orig_type = phrase_nodes->at(index)->type;

                phrase_nodes->at(index)->type = NEGATEXPR;

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // NEGATEXPR_H_INCLUDED
