/**
 * File:            formaltypedecl.h
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
#ifndef FORMALTYPEDECL_H_INCLUDED
#define FORMALTYPEDECL_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        bool lexer::formaltypedecl()
        {
            if ((phrase_nodes->is_valid(index-2) &&
                 (phrase_nodes->at(index-2)->type == ident::IDENTIFIER) &&
                 (phrase_nodes->at(index-1)->type == ident::LPARENTH)) ||
                (phrase_nodes->is_valid(index-2) &&
                 ((phrase_nodes->at(index-2)->type == FORMALVARDECL) ||
                  (phrase_nodes->at(index-2)->type == FORMALTYPEDECL)) &&
                 (phrase_nodes->at(index-1)->type == ident::COMMA)))
            {

                if (phrase_nodes->is_valid(index+1) &&
                    (phrase_nodes->at(index)->type == ident::IDENTIFIER) &&
                    (phrase_nodes->at(index+1)->type == ident::IDENTIFIER))
                {
                    phrase_t* node = new phrase_t();

                    node->type = FORMALTYPEDECL;

                    node->line = phrase_nodes->at(index)->line;
                    node->column = phrase_nodes->at(index)->column;

                    phrase_nodes->at(index)->parent = node;
                    phrase_nodes->at(index+1)->parent = node;

                    node->children.add(phrase_nodes->at(index));
                    node->children.add(phrase_nodes->at(index+1));

                    node->file = phrase_nodes->at(index)->file;

                    phrase_nodes->replace(index, index+1, node);

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // FORMALTYPEDECL_H_INCLUDED
