/**
 * File:            formaldecllist.h
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
#ifndef FORMALDECLLIST_H_INCLUDED
#define FORMALDECLLIST_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class formaldecllist : public syntaxRule
        {
        public:
            ~formaldecllist() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*) const;
        };


        bool formaldecllist::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer) const
        {
            if (phrase_nodes->is_valid(index+2))
            {
                if (((phrase_nodes->at(index)->type == FORMALVARDECL) ||
                     (phrase_nodes->at(index)->type == FORMALTYPEDECL)) &&
                    (phrase_nodes->at(index+1)->type == ident::COMMA) &&
                    ((phrase_nodes->at(index+2)->type == FORMALVARDECL) ||
                     (phrase_nodes->at(index+2)->type == FORMALTYPEDECL)))
                {
                    phrase_t* node = new phrase_t();

                    node->type = FORMALDECLLIST;

                    node->line = phrase_nodes->at(index)->line;
                    node->column = phrase_nodes->at(index)->column;

                    phrase_nodes->at(index)->parent = node;
                    phrase_nodes->at(index+2)->parent = node;

                    node->children.add(phrase_nodes->at(index));
                    node->children.add(phrase_nodes->at(index+2));

                    node->file = phrase_nodes->at(index)->file;

                    delete phrase_nodes->at(index+1);
                    phrase_nodes->replace(index, index+2, node);

                    return true;
                }
                else if ((phrase_nodes->at(index)->type == FORMALDECLLIST) &&
                         (phrase_nodes->at(index+1)->type == ident::COMMA) &&
                         ((phrase_nodes->at(index+2)->type == FORMALVARDECL) ||
                          (phrase_nodes->at(index+2)->type == FORMALTYPEDECL)))
                {
                    phrase_nodes->at(index)->children.add(phrase_nodes->at(index+2));
                    phrase_nodes->at(index+2)->parent = phrase_nodes->at(index);

                    delete phrase_nodes->at(index+1);
                    phrase_nodes->remove(index+2);
                    phrase_nodes->remove(index+1);

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // FORMALDECLLIST_H_INCLUDED
