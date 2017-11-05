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
        class variable : public syntaxRule
        {
        public:
            ~variable() {}

            bool apply(core::array< phrase_t* >*,
                       int);
        };

        bool variable::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index)
        {
            /*if (((phrase_nodes->at(index)->type == ident::IDENTIFIER) &&
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
                        (phrase_nodes->at(index+2)->metaValue == KEYWORD::DIM)) ))*/
            if ((phrase_nodes->at(index)->type == ident::IDENTIFIER) &&
                !(phrase_nodes->is_valid(index+1) &&
                  ((phrase_nodes->at(index+1)->type == ident::LPARENTH) ||
                   (phrase_nodes->at(index+1)->type == ident::IDENTIFIER) ||
                   (phrase_nodes->at(index+1)->type == ident::LBRACE)
                   )
                  ) &&
                !(phrase_nodes->is_valid(index+2) &&
                  (phrase_nodes->at(index+1)->type == ident::PERIOD) &&
                  (phrase_nodes->at(index+2)->type == ident::KEYWORD) &&
                  (phrase_nodes->at(index+2)->metaValue == DIM)
                  )
                )
            {
                phrase_t* pIndex = phrase_nodes->at(index);

                phrase_t* node =
                    new phrase_t(*pIndex, VARIABLE);

                pIndex->parent = node;
                node->children.add(pIndex);

                phrase_nodes->at(index) = node;

                //std::cout << node->line << std::endl;

                return true;
            }
            /*else if ((phrase_nodes->at(index)->type == TYPEVAR) ||
                     ((phrase_nodes->at(index)->type == VARINDEX) &&
                      !(phrase_nodes->is_valid(index+1) &&
                       (phrase_nodes->at(index+1)->type == ident::LBRACKET))))
            {
                if (phrase_nodes->at(index)->orig_type == ident::NONE)
                    phrase_nodes->at(index)->orig_type = phrase_nodes->at(index)->type;
                phrase_nodes->at(index)->type = VARIABLE;

                return true;
            }*/

            return false;
        }
    }
}
}

#endif // VARIABLE_H_INCLUDED
