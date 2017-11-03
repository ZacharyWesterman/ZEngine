/**
 * File:            boolexpr.h
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
 * Last modified:   21 Jul. 2017
**/

#pragma once
#ifndef BOOLEXPR_H_INCLUDED
#define BOOLEXPR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        class boolexpr : public syntaxRule<CHAR>
        {
        public:
            ~boolexpr() {}

            bool apply(core::array< phrase_t<CHAR>* >*,
                       int);
        };

        template <typename CHAR>
        bool boolexpr<CHAR>::apply(core::array< phrase_t<CHAR>* >* phrase_nodes,
                                  int index)
        {
            //if no detected boolean operators, continue to the next phase
            if ((phrase_nodes->at(index)->type == ADDEXPR) &&
                 !(phrase_nodes->is_valid(index+1) &&
                   (phrase_nodes->at(index+1)->type == ident::OPERATOR) &&
                   (phrase_nodes->at(index+1)->metaValue >= AND_LGCL) &&
                   (phrase_nodes->at(index+1)->metaValue <= LT_EQ)
                   ) &&
                 !(phrase_nodes->is_valid(index-1) &&
                   (phrase_nodes->at(index-1)->type == ident::OPERATOR) &&
                   (phrase_nodes->at(index-1)->metaValue >= AND_LGCL) &&
                   (phrase_nodes->at(index-1)->metaValue <= LT_EQ)
                   )
                 )
            {
                setSuperType(phrase_nodes->at(index), BOOLEXPR);

                return true;
            }
            //otherwise, if a binary boolean operator is detected
            else if (phrase_nodes->is_valid(index+2) &&
                     ((phrase_nodes->at(index)->type == ADDEXPR) ||
                      (phrase_nodes->at(index)->type == BOOLEXPR)
                      ) &&
                     (phrase_nodes->at(index+1)->type >= ident::OPERATOR) &&
                     (phrase_nodes->at(index+1)->metaValue >= AND_LGCL) &&
                     (phrase_nodes->at(index+1)->metaValue <= LT_EQ) &&
                     (phrase_nodes->at(index+2)->type == ADDEXPR)
                     )
            {
                phrase_t<CHAR>* node =
                    new phrase_t<CHAR>(*(phrase_nodes->at(index)), BOOLEXPR);

                phrase_nodes->at(index)->parent = node;
                phrase_nodes->at(index+1)->parent = node;
                phrase_nodes->at(index+2)->parent = node;

                node->children.add(phrase_nodes->at(index));
                node->children.add(phrase_nodes->at(index+1));
                node->children.add(phrase_nodes->at(index+2));

                phrase_nodes->replace(index, index+2, node);

                return true;
            }
            //or a unary boolean operator
            else if (phrase_nodes->is_valid(index+1) &&
                     ((phrase_nodes->at(index+1)->type == ADDEXPR) ||
                      (phrase_nodes->at(index+1)->type == BOOLEXPR)
                      ) &&
                     (phrase_nodes->at(index)->type == ident::OPERATOR) &&
                     ((phrase_nodes->at(index)->metaValue == NOT_LGCL) ||
                      (phrase_nodes->at(index)->metaValue == NOT_BITW)
                      )
                     )
            {
                phrase_t<CHAR>* node =
                    new phrase_t<CHAR>(*(phrase_nodes->at(index)), BOOLEXPR);

                phrase_nodes->at(index+1)->parent = node;
                phrase_nodes->at(index)->parent = node;

                node->children.add(phrase_nodes->at(index+1));
                node->children.add(phrase_nodes->at(index));

                phrase_nodes->replace(index, index+1, node);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // BOOLEXPR_H_INCLUDED
