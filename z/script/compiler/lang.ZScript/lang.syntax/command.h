/**
 * File:            command.h
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
 * Last modified:   2 Aug. 2017
**/

#pragma once
#ifndef _COMMAND_H_INCLUDED
#define _COMMAND_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        class _command : public syntaxRule
        {
        public:
            ~_command() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*) const;
        };

        bool _command::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer) const
        {
            if (phrase_nodes->is_valid(index+3) &&
                (phrase_nodes->at(index)->type == IDENTIFIERLIST) &&
                (phrase_nodes->at(index+1)->type == ident::LBRACE) &&
                ((phrase_nodes->at(index+2)->type == EXPRLIST) ||
                 (phrase_nodes->at(index+2)->type == BOOLEXPR)) &&
                (phrase_nodes->at(index+3)->type == ident::RBRACE))
            {
                phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), COMMAND);

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
                (phrase_nodes->at(index)->type == IDENTIFIERLIST) &&
                (phrase_nodes->at(index+1)->type == ident::LBRACE) &&
                (phrase_nodes->at(index+2)->type == ident::RBRACE))
            {
                phrase_t* node =
                    new phrase_t(*(phrase_nodes->at(index)), COMMAND);

                phrase_nodes->at(index)->parent = node;

                node->children.add(phrase_nodes->at(index));

                delete phrase_nodes->at(index+1);
                delete phrase_nodes->at(index+2);
                phrase_nodes->replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // _COMMAND_H_INCLUDED
