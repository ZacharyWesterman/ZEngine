/**
 * File:            error_for.h
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
#ifndef ERROR_FOR_H_INCLUDED
#define ERROR_FOR_H_INCLUDED

namespace z
{
namespace script
{
    namespace compiler
    {

        template <typename CHAR>
        bool lexer<CHAR>::error_for()
        {
            //incomplete or invalid FOR statements
            if (phrase_nodes[index]->type == ident::KEYWORD_FOR)
            {
                if (phrase_nodes.is_valid(index+1) &&
                    (phrase_nodes[index+1]->type == ident::LPARENTH))
                {
                    int param_ct = 1;

                    int i=index+2;
                    while (phrase_nodes[i]->type != ident::RPARENTH)
                    {
                        if (phrase_nodes[i]->type == ident::SEMICOLON)
                            param_ct++;

                        i++;
                    }

                    if (phrase_nodes[i-1]->type == ident::SEMICOLON)
                        param_ct--;


                    if (param_ct > 3)
                        error_buffer.add(parser_error<CHAR>(phrase_nodes[index]->line,
                                                    phrase_nodes[index]->column,
                                                    error::TOO_MANY_PARAMS,
                                                    phrase_nodes[index]->file
                                                    ));
                    else if (param_ct < 3)
                        error_buffer.add(parser_error<CHAR>(phrase_nodes[index]->line,
                                                    phrase_nodes[index]->column,
                                                    error::TOO_FEW_PARAMS,
                                                    phrase_nodes[index]->file
                                                    ));
                    else //no errors in parameters
                    {
                        index = i;

                        //if (phrase_nodes.is_valid())

                        return false;
                    }

                    for (int j=i; j>=index; j--)
                    {
                        delete_ast(phrase_nodes[j]);
                        phrase_nodes.remove(j);
                    }
                }
                else
                {
                    error_buffer.add(parser_error<CHAR>(phrase_nodes[index]->line,
                                                    phrase_nodes[index]->column,
                                                    error::EXPECTED_PARAMETER,
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

#endif // ERROR_FOR_H_INCLUDED
