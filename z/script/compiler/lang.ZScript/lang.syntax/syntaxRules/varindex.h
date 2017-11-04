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
            if (phrase_nodes.is_valid(index+1) &&
                ((phrase_nodes[index]->type == phrase::PARENTHEXPR) ||
                 (phrase_nodes[index]->type == phrase::VARINDEX) ||
                 (phrase_nodes[index]->type == phrase::LIST) ||
                 ((phrase_nodes[index]->type == ident::LITERAL) &&
                  ((phrase_nodes[index]->value.type() == data::STRING) ||
                   (phrase_nodes[index]->value.type() == data::ARRAY)))) &&
                (phrase_nodes[index+1]->type == phrase::INDEX)
                )
            {
                phrase_t* node = new phrase_t();

                node->type = phrase::VARINDEX;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;
                phrase_nodes[index+1]->parent = node;

                node->children.add(phrase_nodes[index]);
                node->children.add(phrase_nodes[index+1]);

                node->file = phrase_nodes[index]->file;

                //check if this is a sub-index
                //(previous 'index' was an index list)
                if ((phrase_nodes[index]->type == phrase::VARINDEX) &&
                    (phrase_nodes[index]->children[1]->type==phrase::INDEX) &&
                    (phrase_nodes[index]->children[1]->children.size() > 1))
                {
                    //indicate this is a sub-index
                    node->metaValue = 1;
                }


                phrase_nodes.replace(index, index+1, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // VARINDEX_H_INCLUDED
