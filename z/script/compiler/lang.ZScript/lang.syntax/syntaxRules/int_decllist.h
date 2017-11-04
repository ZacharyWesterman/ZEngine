/**
 * File:            int_decllist.h
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
 * Last modified:   29 Aug. 2017
**/

#pragma once
#ifndef INT_DECLLIST_H_INCLUDED
#define INT_DECLLIST_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        bool lexer::int_decllist()
        {
            if (((phrase_nodes->is_valid(index-4) &&
                 (phrase_nodes->at(index-4)->type == ident::KEYWORD_TYPE) &&
                 (phrase_nodes->at(index-3)->type == ident::IDENTIFIER) &&
                 (phrase_nodes->at(index-2)->type == ident::LBRACE) &&
                 ((phrase_nodes->at(index-1)->type == VARIABLE_DECL) ||
                  (phrase_nodes->at(index-1)->type == TYPEVAR_DECL) ||
                  (phrase_nodes->at(index-1)->type == FUNCTION_DECL))) ) &&
                ((phrase_nodes->at(index)->type == VARIABLE_DECL) ||
                 (phrase_nodes->at(index)->type == TYPEVAR_DECL) ||
                 (phrase_nodes->at(index)->type == FUNCTION_DECL)))
            {
                phrase_t* node = new phrase_t();

                node->type = INT_DECLLIST;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                phrase_nodes->at(index-1)->parent = node;
                phrase_nodes->at(index)->parent = node;

                node->children.add(phrase_nodes->at(index-1));
                node->children.add(phrase_nodes->at(index));

                node->file = phrase_nodes->at(index-1)->file;

                phrase_nodes->replace(index-1, index, node);

                return true;
            }
            else if (phrase_nodes->is_valid(index-1) &&
                     (phrase_nodes->at(index-1)->type == INT_DECLLIST) &&

                     ((phrase_nodes->at(index)->type == VARIABLE_DECL) ||
                      (phrase_nodes->at(index)->type == TYPEVAR_DECL) ||
                      (phrase_nodes->at(index)->type == FUNCTION_DECL)) )
            {
                phrase_nodes->at(index-1)->children.add(phrase_nodes->at(index));
                phrase_nodes->at(index)->parent = phrase_nodes->at(index-1);
                phrase_nodes->remove(index);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // INT_DECLLIST_H_INCLUDED
