/**
 * File:            funccall.h
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
#ifndef FUNCCALL_H_INCLUDED
#define FUNCCALL_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        class funccall : public syntaxRule
        {
        public:
            ~funccall() {}

            bool apply(core::array< phrase_t* >*,
                       int);
        };


        bool funccall::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index)
        {
            if (phrase_nodes->is_valid(index+3) &&
                (phrase_nodes->at(index)->type == ident::IDENTIFIER) &&
                ((phrase_nodes->at(index+2)->type == BOOLEXPR) ||
                 (phrase_nodes->at(index+2)->type == EXPRLIST)) &&
                (phrase_nodes->at(index+1)->type == ident::LPARENTH) &&
                (phrase_nodes->at(index+3)->type == ident::RPARENTH) &&
                !(phrase_nodes->is_valid(index+4) &&
                 (phrase_nodes->at(index+4)->type == ident::LBRACE)))
            {
                phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), FUNCCALL);

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+2)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+2));

                delete phrase_nodes->at(index+1);
                delete phrase_nodes->at(index+3);
                phrase_nodes->replace(index, index+3, node);

                return true;
            }
            else if (phrase_nodes->is_valid(index+2) &&
                    (phrase_nodes->at(index)->type == ident::IDENTIFIER) &&
                    (phrase_nodes->at(index+1)->type == ident::LPARENTH) &&
                    (phrase_nodes->at(index+2)->type == ident::RPARENTH) &&
                    !(phrase_nodes->is_valid(index+3) &&
                      (phrase_nodes->at(index+3)->type == ident::LBRACE)))
            {
                phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), FUNCCALL);

                phrase_nodes->at(index)->parent = node;

                node->children.add(phrase_nodes->at(index));

                delete phrase_nodes->at(index+1);
                delete phrase_nodes->at(index+2);
                phrase_nodes->replace(index, index+2, node);

                return true;
            }

            return false;
        }
    }
}
}

#endif // FUNCCALL_H_INCLUDED
