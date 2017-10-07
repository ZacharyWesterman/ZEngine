/**
 * File:            index.h
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
#ifndef INDEX_H_INCLUDED
#define INDEX_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        bool lexer<CHAR>::_index()
        {
            if (phrase_nodes.is_valid(index+2) &&
                (phrase_nodes[index]->type == ident::LBRACKET) &&
                ((phrase_nodes[index+1]->type == phrase::BOOLEXPR) ||
                 (phrase_nodes[index+1]->type == phrase::RANGE) ||
                 (phrase_nodes[index+1]->type == phrase::INDEXLIST)) &&
                (phrase_nodes[index+2]->type == ident::RBRACKET))
            {
                if (phrase_nodes[index+1]->orig_type == ident::NONE)
                    phrase_nodes[index+1]->orig_type =
                                            phrase_nodes[index+1]->type;
                phrase_nodes[index+1]->type = phrase::INDEX;

                delete phrase_nodes[index];
                delete phrase_nodes[index+2];

                phrase_nodes.remove(index+2);
                phrase_nodes.remove(index);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // INDEX_H_INCLUDED
