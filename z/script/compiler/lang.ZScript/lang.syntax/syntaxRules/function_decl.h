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

        bool lexer::function_decl()
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
                phrase_t* node = new phrase_t();

                node->type = FUNCTION_DECL;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+2)->parent = node;
                phrase_nodes->at(index+5)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+2));
                node->children.add(phrase_nodes->at(index+5));

                node->file = phrase_nodes->at(index)->file;

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
                phrase_t* node = new phrase_t();

                node->type = FUNCTION_DECL;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+4)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+4));

                node->file = phrase_nodes->at(index)->file;

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
                phrase_t* node = new phrase_t();

                node->type = FUNCTION_DECL;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+2)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+2));

                node->file = phrase_nodes->at(index)->file;

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
                phrase_t* node = new phrase_t();

                node->type = FUNCTION_DECL;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                phrase_nodes->at(index)->parent = node;

                node->children.add(phrase_nodes->at(index));

                node->file = phrase_nodes->at(index)->file;

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
                if (phrase_nodes->at(index-1)->type == ident::KEYWORD_VAR)
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
