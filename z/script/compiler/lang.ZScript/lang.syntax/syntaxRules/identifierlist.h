/**
 * File:            identifierlist.h
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
#ifndef IDENTIFIERLIST_H_INCLUDED
#define IDENTIFIERLIST_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        bool lexer::identifierlist()
        {
            if (!(phrase_nodes->is_valid(index-1) &&
                  ((phrase_nodes->at(index-1)->type == ident::KEYWORD_IN) ||
                   (phrase_nodes->at(index-1)->type == ident::KEYWORD_SUB) ||
                   (phrase_nodes->at(index-1)->type == ident::KEYWORD_TYPE) ||
                   (phrase_nodes->at(index-1)->type == ident::LPARENTH) ||
                   (phrase_nodes->at(index-1)->type == ident::COMMA) )))
            {
                if (phrase_nodes->is_valid(index+1) &&
                    ((phrase_nodes->at(index)->type == ident::IDENTIFIER) ||
                     (phrase_nodes->at(index)->type == IDENTIFIERLIST)) &&
                    (phrase_nodes->at(index+1)->type == ident::IDENTIFIER) &&
                    !(phrase_nodes->is_valid(index+2) &&
                      ((phrase_nodes->at(index+2)->type == ident::SEMICOLON) ||
                       (phrase_nodes->at(index+2)->type == ident::OPER_ASSIGN) ||
                       (phrase_nodes->at(index+2)->type == ident::LPARENTH))))
                {
                    if (phrase_nodes->at(index)->type == IDENTIFIERLIST)
                    {
                        phrase_nodes->at(index)->children.add(phrase_nodes->at(index+1));

                        phrase_nodes->at(index+1)->parent = phrase_nodes->at(index);

                        phrase_nodes->remove(index+1);
                    }
                    else
                    {
                        phrase_t* node = new phrase_t();

                        node->type = IDENTIFIERLIST;

                        node->line = phrase_nodes->at(index)->line;
                        node->column = phrase_nodes->at(index)->column;

                        phrase_nodes->at(index)->parent = node;
                        phrase_nodes->at(index+1)->parent = node;

                        node->children.add(phrase_nodes->at(index));
                        node->children.add(phrase_nodes->at(index+1));

                        node->file = phrase_nodes->at(index)->file;

                        phrase_nodes->replace(index, index+1, node);
                    }

                    return true;
                }
                else if (phrase_nodes->is_valid(index+1) &&
                         (phrase_nodes->at(index)->type == ident::IDENTIFIER) &&
                         (phrase_nodes->at(index+1)->type == ident::LBRACE))
                {
                    phrase_t* node = new phrase_t();

                    node->type = IDENTIFIERLIST;

                    node->line = phrase_nodes->at(index)->line;
                    node->column = phrase_nodes->at(index)->column;

                    phrase_nodes->at(index)->parent = node;

                    node->children.add(phrase_nodes->at(index));

                    node->file = phrase_nodes->at(index)->file;

                    phrase_nodes->at(index) = node;

                    return true;
                }
            }

            return false;
        }
    }
}
}

#endif // IDENTIFIERLIST_H_INCLUDED
