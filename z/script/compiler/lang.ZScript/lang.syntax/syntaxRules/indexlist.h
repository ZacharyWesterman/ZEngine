/**
 * File:            indexlist.h
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
#ifndef INDEXLIST_H_INCLUDED
#define INDEXLIST_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        bool lexer::indexlist()
        {
            if (phrase_nodes->is_valid(index+2) &&
                phrase_nodes->is_valid(index-1) &&
                (phrase_nodes->at(index-1)->type == ident::LBRACKET) &&
                (phrase_nodes->at(index)->type == INDEXLIST) &&
                ((phrase_nodes->at(index+1)->type == RANGE) ||
                 (phrase_nodes->at(index+1)->type == BOOLEXPR)) &&
                ((phrase_nodes->at(index+2)->type == ident::COMMA) ||
                 (phrase_nodes->at(index+2)->type == ident::RBRACKET)))
            {
                phrase_nodes->at(index)->children.add(phrase_nodes->at(index+1));
                phrase_nodes->at(index+1)->parent = phrase_nodes->at(index);


                if (phrase_nodes->at(index+2)->type == ident::COMMA)
                {
                    delete phrase_nodes->at(index+2);
                    phrase_nodes->remove(index+2);
                    phrase_nodes->remove(index+1);
                }
                else
                {
                    phrase_nodes->remove(index+1);
                }

                return true;
            }
            else if (phrase_nodes->is_valid(index+1) &&
                phrase_nodes->is_valid(index-1) &&
                (phrase_nodes->at(index-1)->type == ident::LBRACKET) &&
                ((phrase_nodes->at(index)->type == RANGE) ||
                 (phrase_nodes->at(index)->type == BOOLEXPR)) &&
                (phrase_nodes->at(index+1)->type == ident::COMMA))
            {
                phrase_t* node = new phrase_t();

                node->type = INDEXLIST;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                phrase_nodes->at(index)->parent = node;

                node->children.add(phrase_nodes->at(index));

                node->file = phrase_nodes->at(index)->file;

                delete phrase_nodes->at(index+1);
                phrase_nodes->replace(index, index+1, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // INDEXLIST_H_INCLUDED
