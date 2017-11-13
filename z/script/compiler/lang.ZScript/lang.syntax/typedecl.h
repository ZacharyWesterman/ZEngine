/**
 * File:            typedecl.h
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
 * Last modified:   27 Aug. 2017
**/

#pragma once
#ifndef TYPEDECL_H_INCLUDED
#define TYPEDECL_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class typedecl : public syntaxRule
        {
        public:
            ~typedecl() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*) const;
        };

        bool typedecl::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer) const
        {
            //type IDENTIFIER { ... } ;
            if (phrase_nodes->is_valid(index+5) &&
                (phrase_nodes->at(index)->type == ident::KEYWORD) &&
                (phrase_nodes->at(index)->metaValue == TYPE) &&
                (phrase_nodes->at(index+1)->type == ident::IDENTIFIER) &&
                (phrase_nodes->at(index+2)->type == ident::LBRACE) &&
                ((phrase_nodes->at(index+3)->type == VARIABLE_DECL) ||
                 (phrase_nodes->at(index+3)->type == TYPEVAR_DECL) ||
                 (phrase_nodes->at(index+3)->type == FUNC_PROTOTYPE) ||
                 (phrase_nodes->at(index+3)->type == FUNCTION_DECL) ||
                 (phrase_nodes->at(index+3)->type == INT_DECLLIST)
                 ) &&
                (phrase_nodes->at(index+4)->type == ident::RBRACE) &&
                (phrase_nodes->at(index+5)->type == ident::SEMICOLON)
                )
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), TYPEDECL);

                phrase_nodes->at(index+1)->parent = node;
                phrase_nodes->at(index+3)->parent = node;

                node->children.add(phrase_nodes->at(index+1));
                node->children.add(phrase_nodes->at(index+3));

                delete phrase_nodes->at(index);
                delete phrase_nodes->at(index+2);
                delete phrase_nodes->at(index+4);
                phrase_nodes->replace(index, index+4, node);

                return true;
            }
            //type IDENTIFIER { } ;
            else if (phrase_nodes->is_valid(index+4) &&
                     (phrase_nodes->at(index)->type == ident::KEYWORD) &&
                     (phrase_nodes->at(index)->metaValue == TYPE) &&
                     (phrase_nodes->at(index+1)->type == ident::IDENTIFIER) &&
                     (phrase_nodes->at(index+2)->type == ident::LBRACE) &&
                     (phrase_nodes->at(index+3)->type == ident::RBRACE) &&
                     (phrase_nodes->at(index+4)->type == ident::SEMICOLON)
                     )
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), TYPEDECL);

                phrase_nodes->at(index+1)->parent = node;

                node->children.add(phrase_nodes->at(index+1));

                delete phrase_nodes->at(index);
                delete phrase_nodes->at(index+2);
                delete phrase_nodes->at(index+3);
                phrase_nodes->replace(index, index+3, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // TYPEDECL_H_INCLUDED
