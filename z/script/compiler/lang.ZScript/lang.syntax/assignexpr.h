/**
 * File:            assignexpr.h
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
#ifndef ASSIGNEXPR_H_INCLUDED
#define ASSIGNEXPR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        class assignexpr : public syntaxRule<CHAR>
        {
        public:
            ~assignexpr() {}

            bool apply(core::array< phrase_t<CHAR>* >*,
                       int);
        };

        template <typename CHAR>
        bool assignexpr<CHAR>::apply(core::array< phrase_t<CHAR>* >* phrase_nodes,
                                  int index)
        {
            if (phrase_nodes->is_valid(index+2) &&
                (phrase_nodes->at(index)->type == VARIABLE) &&
                (phrase_nodes->at(index+1)->type == ident::OPERATOR) &&
                (phrase_nodes->at(index+1)->metaValue >= ASSIGN) &&
                (phrase_nodes->at(index+1)->metaValue <= MOD_ASSIGN) &&
                (phrase_nodes->at(index+2)->type == BOOLEXPR)
                )
            {
                phrase_t<CHAR>* node =
                    new phrase_t<CHAR>(*(phrase_nodes->at(index)), ASSIGNEXPR);

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+1)->parent = node;
                phrase_nodes->at(index+2)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+1));
                node->children.add(phrase_nodes->at(index+2));

                phrase_nodes->replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // ASSIGNEXPR_H_INCLUDED
