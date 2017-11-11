/**
 * File:            identifierlist.h
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
 * Last modified:   23 Aug. 2017
**/

#pragma once
#ifndef IDENTIFIERLIST_H_INCLUDED
#define IDENTIFIERLIST_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        class identifierlist : public syntaxRule
        {
        public:
            ~identifierlist() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*);
        };

        bool identifierlist::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer)
        {
            if (!(phrase_nodes->is_valid(index-1) &&
                  ((phrase_nodes->at(index-1)->type == ident::KEYWORD) ||
                   //(phrase_nodes->at(index-1)->type == ident::LPARENTH) ||
                   (phrase_nodes->at(index-1)->type == ident::COMMA)
                   )
                  )
                )
            {
                if (phrase_nodes->is_valid(index+1) &&
                    ((phrase_nodes->at(index)->type == ident::IDENTIFIER) ||
                     (phrase_nodes->at(index)->type == IDENTIFIERLIST)
                     ) &&
                    (phrase_nodes->at(index+1)->type == ident::IDENTIFIER) &&
                    phrase_nodes->is_valid(index+2) &&
                    ((phrase_nodes->at(index+2)->type == ident::IDENTIFIER) ||
                     (phrase_nodes->at(index+2)->type == ident::LBRACE)
                     )
                    )
                {
                    if (phrase_nodes->at(index)->type == IDENTIFIERLIST)
                    {
                        phrase_nodes->at(index)->children.add(phrase_nodes->at(index+1));

                        phrase_nodes->at(index+1)->parent = phrase_nodes->at(index);

                        phrase_nodes->remove(index+1);
                    }
                    else
                    {
                        phrase_t* node =
                            new phrase_t(*(phrase_nodes->at(index)), IDENTIFIERLIST);

                        phrase_nodes->at(index)->parent = node;
                        phrase_nodes->at(index+1)->parent = node;

                        node->children.add(phrase_nodes->at(index));
                        node->children.add(phrase_nodes->at(index+1));

                        phrase_nodes->replace(index, index+1, node);
                    }

                    return true;
                }
                else if (phrase_nodes->is_valid(index+1) &&
                         (phrase_nodes->at(index)->type == ident::IDENTIFIER) &&
                         (phrase_nodes->at(index+1)->type == ident::LBRACE))
                {
                    phrase_t* node =
                            new phrase_t(*(phrase_nodes->at(index)), IDENTIFIERLIST);

                    phrase_nodes->at(index)->parent = node;

                    node->children.add(phrase_nodes->at(index));

                    phrase_nodes->at(index) = node;

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // IDENTIFIERLIST_H_INCLUDED
