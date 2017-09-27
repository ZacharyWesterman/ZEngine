/**
 * File:            error_semicolon.h
 * Namespace:       z::script
 * Description:     Implementation of the lexer's
 *                  error detecting member functions.
 *                  This file should NOT be included
 *                  in a project. Include "lexer.h"
 *                  instead,and put this file in the
 *                  "syntaxErrors" folder.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   8 Aug. 2017
**/

#pragma once
#ifndef ERROR_SEMICOLON_H_INCLUDED
#define ERROR_SEMICOLON_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        template <typename CHAR>
        bool lexer<CHAR>::error_semicolon()
        {
            if (phrase_nodes[index]->type == ident::SEMICOLON)
            {
                //statements outside of function declaration
                if (phrase_nodes.is_valid(index-1) &&
                    (phrase_nodes[index-1]->type == phrase::BOOLEXPR))
                {
                    error_buffer.add(error(phrase_nodes[index]->line,
                                                    phrase_nodes[index]->column,
                                                    error::STMT_OUTSIDE_FUNCTION,
                                                    phrase_nodes[index]->file
                                                    ));

                    delete phrase_nodes[index];
                    delete_ast(phrase_nodes[index-1]);

                    phrase_nodes.remove(index);
                    phrase_nodes.remove(index-1);
                }
                //misplaced semicolons
                else
                {
                    error_buffer.add(error(phrase_nodes[index]->line,
                                                    phrase_nodes[index]->column,
                                                    error::UNEXPECTED_SEMICOLON,
                                                    phrase_nodes[index]->file
                                                    ));

                    delete phrase_nodes[index];
                    phrase_nodes.remove(index);
                }

                return true;
            }


            return false;
        }

    }
}
}

#endif // ERROR_SEMICOLON_H_INCLUDED
