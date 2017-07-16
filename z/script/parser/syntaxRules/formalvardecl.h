/**
 * File:            formalvardecl.h
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
#ifndef FORMALVARDECL_H_INCLUDED
#define FORMALVARDECL_H_INCLUDED

namespace z
{
    namespace script
    {
        template <typename CHAR>
        bool lexer<CHAR>::formalvardecl()
        {
            if ((phrase_nodes.is_valid(index-3) &&
                 (phrase_nodes[index-3]->type == ident::KEYWORD_FUNCTION) &&
                 (phrase_nodes[index-2]->type == ident::IDENTIFIER) &&
                 (phrase_nodes[index-1]->type == ident::LPARENTH)) ||
                (phrase_nodes.is_valid(index-2) &&
                 ((phrase_nodes[index-2]->type == phrase::FORMALVARDECL) ||
                  (phrase_nodes[index-2]->type == phrase::FORMALTYPEDECL)) &&
                 (phrase_nodes[index-1]->type == ident::COMMA)))
            {

                if (phrase_nodes.is_valid(index+1) &&
                    (phrase_nodes[index]->type == ident::KEYWORD_VAR) &&
                    (phrase_nodes[index+1]->type == ident::IDENTIFIER))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::FORMALVARDECL;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index+1]->parent = node;

                    node->children.add(phrase_nodes[index+1]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    phrase_nodes.replace(index, index+1, node);

                    return true;
                }
                else if ((phrase_nodes[index]->type == ident::IDENTIFIER) &&
                         !(phrase_nodes.is_valid(index+1) &&
                          (phrase_nodes[index+1]->type == ident::IDENTIFIER)))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::FORMALVARDECL;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index]->parent = node;

                    node->children.add(phrase_nodes[index]);

                    node->file = phrase_nodes[index]->file;

                    phrase_nodes[index] = node;

                    return true;
                }
            }

            return false;
        }
    }
}

#endif // FORMALVARDECL_H_INCLUDED
