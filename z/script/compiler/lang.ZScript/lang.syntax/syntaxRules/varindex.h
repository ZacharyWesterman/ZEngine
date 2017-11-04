/**
 * File:            varindex.h
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
#ifndef VARINDEX_H_INCLUDED
#define VARINDEX_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        bool lexer::varindex()
        {
            if (phrase_nodes->is_valid(index+1) &&
                ((phrase_nodes->at(index)->type == PARENTHEXPR) ||
                 (phrase_nodes->at(index)->type == VARINDEX) ||
                 (phrase_nodes->at(index)->type == LIST) ||
                 ((phrase_nodes->at(index)->type == ident::LITERAL) &&
                  ((phrase_nodes->at(index)->value.type() == data::STRING) ||
                   (phrase_nodes->at(index)->value.type() == data::ARRAY)))) &&
                (phrase_nodes->at(index+1)->type == INDEX)
                )
            {
                phrase_t* node = new phrase_t();

                node->type = VARINDEX;

                node->line = phrase_nodes->at(index)->line;
                node->column = phrase_nodes->at(index)->column;

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+1)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+1));

                node->file = phrase_nodes->at(index)->file;

                //check if this is a sub-index
                //(previous 'index' was an index list)
                if ((phrase_nodes->at(index)->type == VARINDEX) &&
                    (phrase_nodes->at(index)->children->at(1)->type==INDEX) &&
                    (phrase_nodes->at(index)->children->at(1)->children.size() > 1))
                {
                    //indicate this is a sub-index
                    node->metaValue = 1;
                }


                phrase_nodes->replace(index, index+1, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // VARINDEX_H_INCLUDED
