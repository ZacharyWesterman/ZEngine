/**
 * File:            addexpr.h
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
 * Last modified:   19 Jul. 2017
**/

#pragma once
#ifndef ADDEXPR_H_INCLUDED
#define ADDEXPR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        class addexpr : public syntaxRule<CHAR>
        {
        public:
            ~addexpr() {}

            bool apply(core::array< phrase_t<CHAR>* >*,
                       int);
        };

        template <typename CHAR>
        bool addexpr<CHAR>::apply(core::array< phrase_t<CHAR>* >* phrase_nodes,
                                  int index)
        {
            //if no detected addition operators, continue to the next phase
            if ((phrase_nodes->at(index)->type == MULTIPLYEXPR) &&
                !(phrase_nodes->is_valid(index+1) &&
                  (phrase_nodes->at(index+1)->type >= ident::OPERATOR) &&
                  ((phrase_nodes->at(index+1)->metaValue == ADD) ||
                   (phrase_nodes->at(index+1)->metaValue == SUB)
                   )
                  ) &&
                !(phrase_nodes->is_valid(index-1) &&
                  (phrase_nodes->at(index-1)->type >= ident::OPERATOR) &&
                  ((phrase_nodes->at(index-1)->metaValue == ADD) ||
                   (phrase_nodes->at(index-1)->metaValue == SUB)
                   )
                  )
                )
            {
                setSuperType(phrase_nodes->at(index), ADDEXPR);

                return true;
            }
             //otherwise, if an addition operator is detected
            else if (phrase_nodes->is_valid(index+2) &&
                     ((phrase_nodes->at(index)->type == MULTIPLYEXPR) ||
                      (phrase_nodes->at(index)->type == ADDEXPR)
                      ) &&
                     (phrase_nodes->at(index+1)->type == ident::OPERATOR) &&
                     ((phrase_nodes->at(index+1)->metaValue == ADD) ||
                      (phrase_nodes->at(index+1)->metaValue == SUB)
                      ) &&
                     (phrase_nodes->at(index+2)->type == MULTIPLYEXPR)
                     )
            {
                phrase_t<CHAR>* node =
                    new phrase_t<CHAR>(*(phrase_nodes->at(index)),
                                       ADDEXPR);

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+1)->parent = node;
                phrase_nodes->at(index+2)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+1));
                node->children.add(phrase_nodes->at(index+2));


                phrase_nodes->replace(index, index+2, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // ADDEXPR_H_INCLUDED
