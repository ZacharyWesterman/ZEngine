/**
 * File:            list.h
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
#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        bool lexer<CHAR>::_list()
        {
            if (!(phrase_nodes.is_valid(index-2) &&
                  (phrase_nodes[index-2]->type == ident::KEYWORD_IN)))
            {
                if (phrase_nodes.is_valid(index+2) &&
                    (phrase_nodes[index]->type == ident::LBRACE) &&
                    (phrase_nodes[index+2]->type == ident::RBRACE))
                {
                    if (phrase_nodes[index+1]->type == phrase::BOOLEXPR)
                    {
                        phrase_t<CHAR>* node = new phrase_t<CHAR>();

                        node->type = phrase::LIST;

                        node->line = phrase_nodes[index]->line;
                        node->column = phrase_nodes[index]->column;

                        node->file = phrase_nodes[index]->file;


                        phrase_nodes[index+1]->parent = node;

                        node->children.add(phrase_nodes[index+1]);


                        delete phrase_nodes[index];
                        delete phrase_nodes[index+2];
                        phrase_nodes.replace(index, index+2, node);

                        return true;
                    }
                    else if (phrase_nodes[index+1]->type == phrase::EXPRLIST)
                    {
                        phrase_nodes[index+1]->type = phrase::LIST;

                        phrase_nodes[index+1]->line = phrase_nodes[index]->line;
                        phrase_nodes[index+1]->column = phrase_nodes[index]->column;

                        phrase_nodes[index+1]->file = phrase_nodes[index]->file;

                        delete phrase_nodes[index];
                        delete phrase_nodes[index+2];
                        phrase_nodes.remove(index+2);
                        phrase_nodes.remove(index);

                        return true;
                    }
                }
                else if (phrase_nodes.is_valid(index+1) &&
                    (phrase_nodes[index]->type == ident::LBRACE) &&
                    (phrase_nodes[index+1]->type == ident::RBRACE) &&
                    !(phrase_nodes.is_valid(index-1) &&
                      ((phrase_nodes[index-1]->type == ident::RPARENTH) ||
                       (phrase_nodes[index-1]->type == ident::KEYWORD_ELSE) ||
                       (phrase_nodes[index-1]->type == ident::KEYWORD_LOOP) ||
                       (phrase_nodes[index-1]->type == ident::KEYWORD_VAR) ||
                       (phrase_nodes[index-1]->type == ident::IDENTIFIER) )))
                {
                    phrase_t<CHAR>* node = new phrase_t<CHAR>();

                    node->type = phrase::LIST;

                    node->line = phrase_nodes[index]->line;
                    node->column = phrase_nodes[index]->column;

                    node->file = phrase_nodes[index]->file;

                    delete phrase_nodes[index];
                    delete phrase_nodes[index+1];
                    phrase_nodes.replace(index, index+1, node);

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // LIST_H_INCLUDED
