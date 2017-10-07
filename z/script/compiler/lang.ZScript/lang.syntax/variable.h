/**
 * File:            variable.h
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
#ifndef VARIABLE_H_INCLUDED
#define VARIABLE_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        class variable : public syntaxRule<CHAR>
        {
        public:
            ~variable() {}

            bool apply(core::array< phrase_t<CHAR>* >*,
                       int);
        };

        template <typename CHAR>
        bool variable<CHAR>::apply(core::array< phrase_t<CHAR>* >* phrase_nodes,
                                  int index)
        {
            if (((phrase_nodes->at(index)->type == ident::IDENTIFIER) &&
                     !(phrase_nodes->is_valid(index-1) &&
                       (((phrase_nodes->at(index-1)->type == ident::KEYWORD) &&
                        ((phrase_nodes->at(index-1)->metaValue == KEYWORD::SUBR) ||
                        (phrase_nodes->at(index-1)->metaValue == KEYWORD::TYPE) ||
                        (phrase_nodes->at(index-1)->metaValue == KEYWORD::VAR))) ||
                        (phrase_nodes->at(index-1)->type == ident::IDENTIFIER))) &&
                     !(phrase_nodes->is_valid(index+1) &&
                       ((phrase_nodes->at(index+1)->type == ident::LBRACKET) ||
                       (phrase_nodes->at(index+1)->type == ident::LBRACE) ||
                       (phrase_nodes->at(index+1)->type == ident::LPARENTH) ||
                       (phrase_nodes->at(index+1)->type == ident::IDENTIFIER))) &&
                     !(phrase_nodes->is_valid(index+2) &&
                       (phrase_nodes->at(index+1)->type == ident::PERIOD) &&
                       (phrase_nodes->at(index+2)->type == ident::KEYWORD) &&
                        (phrase_nodes->at(index+2)->metaValue == KEYWORD::DIM)) ))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = VARIABLE;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                phrase_nodes->at(index)->parent = node;

                node->children.add(phrase_nodes->at(index));

                node->file = phrase_nodes->at(index)->file;

                phrase_nodes->at(index) = node;

                return true;
            }
            else if ((phrase_nodes->at(index)->type == TYPEVAR) ||
                     ((phrase_nodes->at(index)->type == VARINDEX) &&
                      !(phrase_nodes->is_valid(index+1) &&
                       (phrase_nodes->at(index+1)->type == ident::LBRACKET))))
            {
                if (phrase_nodes->at(index)->orig_type == ident::NONE)
                    phrase_nodes->at(index)->orig_type = phrase_nodes->at(index)->type;
                phrase_nodes->at(index)->type = VARIABLE;

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // VARIABLE_H_INCLUDED
