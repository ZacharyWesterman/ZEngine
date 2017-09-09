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

namespace z
{
namespace script
{
    namespace compiler
    {
        template <typename CHAR>
        bool lexer<CHAR>::program()
        {
            if ((phrase_nodes[index]->type == phrase::VARIABLE_DECL) ||
                (phrase_nodes[index]->type == phrase::TYPEVAR_DECL) ||
                (phrase_nodes[index]->type == phrase::FUNCTION_DECL) ||
                (phrase_nodes[index]->type == phrase::TYPEDECL) ||
                (phrase_nodes[index]->type == phrase::EXTERNALDECL) ||
                (phrase_nodes[index]->type == phrase::SHAREDDECL) ||
                (phrase_nodes[index]->type == phrase::SUBROUTINE_DECL))
            {
                phrase_t<CHAR>* node = new phrase_t<CHAR>();

                node->type = phrase::PROGRAM;

                node->line = phrase_nodes[index]->line;
                node->column = phrase_nodes[index]->column;

                phrase_nodes[index]->parent = node;

                node->children.add(phrase_nodes[index]);

                node->file = phrase_nodes[index]->file;

                phrase_nodes[index] = node;

                return true;
            }
            else if (phrase_nodes.is_valid(index+1) &&
                     (phrase_nodes[index]->type == phrase::PROGRAM) &&

                     ((phrase_nodes[index+1]->type == phrase::VARIABLE_DECL) ||
                      (phrase_nodes[index+1]->type == phrase::TYPEVAR_DECL) ||
                      (phrase_nodes[index+1]->type == phrase::FUNCTION_DECL) ||
                      (phrase_nodes[index+1]->type == phrase::TYPEDECL) ||
                      (phrase_nodes[index+1]->type == phrase::EXTERNALDECL) ||
                      (phrase_nodes[index+1]->type == phrase::SHAREDDECL) ||
                      (phrase_nodes[index+1]->type == phrase::SUBROUTINE_DECL)))
            {
                phrase_nodes[index+1]->parent = phrase_nodes[index];

                phrase_nodes[index]->children.add(phrase_nodes[index+1]);

                phrase_nodes.remove(index+1);

                return true;
            }
            else
                return false;
        }
    }
}
}

#endif // PROGRAM_H_INCLUDED
