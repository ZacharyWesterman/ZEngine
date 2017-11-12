/**
 * File:            function_decl.h
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
#ifndef FUNCTION_DECL_H_INCLUDED
#define FUNCTION_DECL_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        class function_decl : public syntaxRule
        {
        public:
            ~function_decl() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*);
        };


        bool function_decl::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer)
        {
            bool result = false;

            //IDENTIFIER ( ... ) { ... }
            if (phrase_nodes->is_valid(index+6) &&
                (phrase_nodes->at(index)->type == ident::IDENTIFIER) &&
                (phrase_nodes->at(index+1)->type == ident::LPARENTH) &&
                ((phrase_nodes->at(index+2)->type == FORMALVARDECL) ||
                 (phrase_nodes->at(index+2)->type == FORMALTYPEDECL) ||
                 (phrase_nodes->at(index+2)->type == FORMALDECLLIST)) &&
                (phrase_nodes->at(index+3)->type == ident::RPARENTH) &&
                (phrase_nodes->at(index+4)->type == ident::LBRACE) &&
                ((phrase_nodes->at(index+5)->type == STATEMENTLIST) ||
                 (phrase_nodes->at(index+5)->type == STATEMENT)) &&
                (phrase_nodes->at(index+6)->type == ident::RBRACE))
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), FUNCTION_DECL);

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+2)->parent = node;
                phrase_nodes->at(index+5)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+2));
                node->children.add(phrase_nodes->at(index+5));

                delete phrase_nodes->at(index+1);
                delete phrase_nodes->at(index+3);
                delete phrase_nodes->at(index+4);
                delete phrase_nodes->at(index+6);
                phrase_nodes->replace(index, index+6, node);

                result = true;
            }
            //IDENTIFIER ( ) { ... }
            else if (phrase_nodes->is_valid(index+5) &&
                (phrase_nodes->at(index)->type == ident::IDENTIFIER) &&
                (phrase_nodes->at(index+1)->type == ident::LPARENTH) &&
                (phrase_nodes->at(index+2)->type == ident::RPARENTH) &&
                (phrase_nodes->at(index+3)->type == ident::LBRACE) &&
                ((phrase_nodes->at(index+4)->type == STATEMENTLIST) ||
                 (phrase_nodes->at(index+4)->type == STATEMENT)) &&
                (phrase_nodes->at(index+5)->type == ident::RBRACE))
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), FUNCTION_DECL);

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+4)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+4));

                delete phrase_nodes->at(index+1);
                delete phrase_nodes->at(index+2);
                delete phrase_nodes->at(index+3);
                delete phrase_nodes->at(index+5);
                phrase_nodes->replace(index, index+5, node);

                result = true;
            }
            //IDENTIFIER ( ... ) { }
            else if (phrase_nodes->is_valid(index+5) &&
                (phrase_nodes->at(index)->type == ident::IDENTIFIER) &&
                (phrase_nodes->at(index+1)->type == ident::LPARENTH) &&
                ((phrase_nodes->at(index+2)->type == FORMALVARDECL) ||
                 (phrase_nodes->at(index+2)->type == FORMALTYPEDECL) ||
                 (phrase_nodes->at(index+2)->type == FORMALDECLLIST)) &&
                (phrase_nodes->at(index+3)->type == ident::RPARENTH) &&
                (phrase_nodes->at(index+4)->type == ident::LBRACE) &&
                (phrase_nodes->at(index+5)->type == ident::RBRACE))
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), FUNCTION_DECL);

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+2)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+2));

                delete phrase_nodes->at(index+1);
                delete phrase_nodes->at(index+3);
                delete phrase_nodes->at(index+4);
                delete phrase_nodes->at(index+5);
                phrase_nodes->replace(index, index+5, node);

                result = true;
            }
            //IDENTIFIER ( ) { }
            else if (phrase_nodes->is_valid(index+4) &&
                (phrase_nodes->at(index)->type == ident::IDENTIFIER) &&
                (phrase_nodes->at(index+1)->type == ident::LPARENTH) &&
                (phrase_nodes->at(index+2)->type == ident::RPARENTH) &&
                (phrase_nodes->at(index+3)->type == ident::LBRACE) &&
                (phrase_nodes->at(index+4)->type == ident::RBRACE))
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), FUNCTION_DECL);

                phrase_nodes->at(index)->parent = node;

                node->children.add(phrase_nodes->at(index));

                delete phrase_nodes->at(index+1);
                delete phrase_nodes->at(index+2);
                delete phrase_nodes->at(index+3);
                delete phrase_nodes->at(index+4);
                phrase_nodes->replace(index, index+4, node);

                result = true;
            }



            if (result &&
                phrase_nodes->is_valid(index-1))
            {
                //explicit function return type "var"
                if ((phrase_nodes->at(index-1)->type == ident::KEYWORD) &&
                    (phrase_nodes->at(index-1)->metaValue == VAR))
                {
                    delete phrase_nodes->at(index-1);
                    phrase_nodes->remove(index-1);
                }
                //specific function return type
                else if (phrase_nodes->at(index-1)->type == ident::IDENTIFIER)
                {
                    phrase_nodes->at(index)->meta = phrase_nodes->at(index-1)->meta;

                    delete phrase_nodes->at(index-1);
                    phrase_nodes->remove(index-1);
                }
            }

            return result;
        }
    }
}
}

#endif // FUNCTION_DECL_H_INCLUDED
