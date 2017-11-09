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
        class varindex : public syntaxRule
        {
        public:
            ~varindex() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*);
        };


        bool varindex::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer)
        {
            if (phrase_nodes->is_valid(index+1) &&
                ((phrase_nodes->at(index)->type == PARENTHEXPR) /*||
                 (phrase_nodes->at(index)->type == VARINDEX) ||
                 ((phrase_nodes->at(index)->type == BOOLEXPR) &&
                  ((phrase_nodes->at(index)->orig_type == LIST) ||
                   ((phrase_nodes->at(index)->orig_type == ident::LITERAL) &&
                    ((phrase_nodes->at(index)->value.type() == data::STRING) ||
                     (phrase_nodes->at(index)->value.type() == data::ARRAY)
                     )
                    )
                   )
                  )*/
                 ) &&
                (phrase_nodes->at(index+1)->type == INDEX)
                )
            {
                phrase_t* node =
                new phrase_t(*(phrase_nodes->at(index)), VARINDEX);

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+1)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+1));

                //check if this is a sub-index
                //(previous 'index' was an index list)
                if ((phrase_nodes->at(index)->orig_type == VARINDEX) &&
                    ((phrase_nodes->at(index)->children.at(1)->orig_type == RANGE) ||
                     (phrase_nodes->at(index)->children.at(1)->orig_type == RANGELIST)
                     )
                    )
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
