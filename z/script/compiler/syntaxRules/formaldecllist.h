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
        template <typename CHAR>
        bool lexer<CHAR>::formaldecllist()
        {
            if (phrase_nodes.is_valid(index+2))
            {
                if (((phrase_nodes[index]->type == phrase::FORMALVARDECL) ||
                     (phrase_nodes[index]->type == phrase::FORMALTYPEDECL)) &&
                    (phrase_nodes[index+1]->type == ident::COMMA) &&
                    ((phrase_nodes[index+2]->type == phrase::FORMALVARDECL) ||
                     (phrase_nodes[index+2]->type == phrase::FORMALTYPEDECL)))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::FORMALDECLLIST;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    phrase_nodes[index]->parent = node;
                    phrase_nodes[index+2]->parent = node;

                    node->children.add(phrase_nodes[index]);
                    node->children.add(phrase_nodes[index+2]);

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index+1];
                    phrase_nodes.replace(index, index+2, node);

                    return true;
                }
                else if ((phrase_nodes[index]->type == phrase::FORMALDECLLIST) &&
                         (phrase_nodes[index+1]->type == ident::COMMA) &&
                         ((phrase_nodes[index+2]->type == phrase::FORMALVARDECL) ||
                          (phrase_nodes[index+2]->type == phrase::FORMALTYPEDECL)))
                {
                    phrase_nodes[index]->children.add(phrase_nodes[index+2]);
                    phrase_nodes[index+2]->parent = phrase_nodes[index];

                    delete phrase_nodes[index+1];
                    phrase_nodes.remove(index+2);
                    phrase_nodes.remove(index+1);

                    return true;
                }
            }

            return false;
        }
    }
}

#endif // FORMALDECLLIST_H_INCLUDED
