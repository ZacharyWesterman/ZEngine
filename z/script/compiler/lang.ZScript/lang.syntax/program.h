/**
 * File:            program.h
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
 * Last modified:   21 Aug. 2017
**/

#pragma once
#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include <z/script/compiler/syntaxRule.h>



namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        class program : public syntaxRule<CHAR>
        {
        public:
            ~program() {}

            bool apply(core::array< phrase_t<CHAR>* >*,
                       int);
        };

        template <typename CHAR>
        bool program<CHAR>::apply(core::array< phrase_t<CHAR>* >* phrase_nodes,
                                  int index)
        {
            if ((phrase_nodes->at(index)->type == PROGRAM) &&
                phrase_nodes->is_valid(index+1)
                )
            {
                phrase_nodes->at(index+1)->parent = phrase_nodes->at(index);
                phrase_nodes->at(index)->children.add(phrase_nodes->at(index+1));

                phrase_nodes->remove(index+1);

                return true;
            }
            else if (phrase_nodes->at(index)->type != PROGRAM)
            {
                phrase_t<CHAR>* pIndex = phrase_nodes->at(index);

                phrase_t<CHAR>* node =
                    new phrase_t<CHAR>(*pIndex, PROGRAM);

                node->type = PROGRAM;

                pIndex->parent = node;
                node->children.add(pIndex);


                phrase_nodes->at(index) = node;
                return true;
            }

            return false;
        }
    }
}
}

#endif // PROGRAM_H_INCLUDED
