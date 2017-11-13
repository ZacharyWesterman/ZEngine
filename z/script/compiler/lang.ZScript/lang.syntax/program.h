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
        class program : public syntaxRule
        {
        public:
            ~program() {}

            bool apply(core::array< phrase_t* >*,
                       int,
                       core::array<error>*) const;
        };

        bool program::apply(core::array< phrase_t* >* phrase_nodes,
                                  int index,
                                  core::array<error>* error_buffer) const
        {
            if (phrase_nodes->is_valid(index+1) &&
                (phrase_nodes->at(index)->type == PROGRAM) &&
                ((phrase_nodes->at(index+1)->type == VARIABLE_DECL) ||
                 (phrase_nodes->at(index+1)->type == TYPEVAR_DECL) ||
                 (phrase_nodes->at(index+1)->type == FUNC_PROTOTYPE) ||
                 (phrase_nodes->at(index+1)->type == FUNCTION_DECL) ||
                 (phrase_nodes->at(index+1)->type == TYPEDECL) ||
                 (phrase_nodes->at(index+1)->type == EXTERNALDECL) ||
                 (phrase_nodes->at(index+1)->type == SHAREDDECL) ||
                 (phrase_nodes->at(index+1)->type == SUBROUTINE_DECL)
                 )
                )
            {
                phrase_nodes->at(index+1)->parent = phrase_nodes->at(index);
                phrase_nodes->at(index)->children.add(phrase_nodes->at(index+1));

                phrase_nodes->remove(index+1);

                return true;
            }
            else if ((phrase_nodes->at(index)->type == VARIABLE_DECL) ||
                     (phrase_nodes->at(index)->type == TYPEVAR_DECL) ||
                     (phrase_nodes->at(index)->type == FUNC_PROTOTYPE) ||
                     (phrase_nodes->at(index)->type == FUNCTION_DECL) ||
                     (phrase_nodes->at(index)->type == TYPEDECL) ||
                     (phrase_nodes->at(index)->type == EXTERNALDECL) ||
                     (phrase_nodes->at(index)->type == SHAREDDECL) ||
                     (phrase_nodes->at(index)->type == SUBROUTINE_DECL)
                     )
            {
                phrase_t* pIndex = phrase_nodes->at(index);

                phrase_t* node =
                    new phrase_t(*pIndex, PROGRAM);

                node->type = PROGRAM;

                pIndex->parent = node;
                node->children.add(pIndex);


                phrase_nodes->at(index) = node;
                return true;
            }
            else if (phrase_nodes->at(index)->type != PROGRAM)
            {
                if (error_buffer->size() == 0)
                    error_buffer->add(newError(phrase_nodes->at(index),
                                               "Syntax error"));
            }

            return false;
        }
    }
}
}

#endif // PROGRAM_H_INCLUDED
