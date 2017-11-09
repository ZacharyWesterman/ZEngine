/**
 * File:            rangelist.h
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
#ifndef RANGELIST_H_INCLUDED
#define RANGELIST_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class rangelist : public syntaxRule
        {
        public:
            ~rangelist() {}

            bool apply(core::array< phrase_t* >*,
                       int);
        };


        bool rangelist::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index)
        {
            if (phrase_nodes->is_valid(index+2) &&
                phrase_nodes->is_valid(index-1) &&
                (phrase_nodes->at(index-1)->type == ident::LBRACKET) &&
                (phrase_nodes->at(index)->type == RANGELIST) &&
                ((phrase_nodes->at(index+1)->type == RANGE) ||
                 (phrase_nodes->at(index+1)->type == BOOLEXPR)) &&
                ((phrase_nodes->at(index+2)->type == ident::COMMA) ||
                 (phrase_nodes->at(index+2)->type == ident::RBRACKET)))
            {
                phrase_nodes->at(index+1)->parent = phrase_nodes->at(index);

                phrase_nodes->at(index)->children.add(phrase_nodes->at(index+1));


                if (phrase_nodes->at(index+2)->type == ident::COMMA)
                {
                    delete phrase_nodes->at(index+2);
                    phrase_nodes->remove(index+2);
                }

                phrase_nodes->remove(index+1);

                return true;
            }
            else if (phrase_nodes->is_valid(index+1) &&
                phrase_nodes->is_valid(index-1) &&
                (phrase_nodes->at(index-1)->type == ident::LBRACKET) &&
                ((phrase_nodes->at(index)->type == RANGE) ||
                 (phrase_nodes->at(index)->type == BOOLEXPR)) &&
                (phrase_nodes->at(index+1)->type == ident::COMMA))
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), RANGELIST);

                phrase_nodes->at(index)->parent = node;

                node->children.add(phrase_nodes->at(index));

                delete phrase_nodes->at(index+1);
                phrase_nodes->replace(index, index+1, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // RANGELIST_H_INCLUDED
