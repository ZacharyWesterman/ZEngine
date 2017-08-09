/**
 * File:            error_oper.h
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
#ifndef ERROR_OPER_H_INCLUDED
#define ERROR_OPER_H_INCLUDED

namespace z
{
    namespace script
    {

        template <typename CHAR>
        bool lexer<CHAR>::error_oper()
        {
            //unexpected operator
            if ((phrase_nodes[index]->type >= ident::OPER_ASSIGN) &&
                (phrase_nodes[index]->type <= ident::OPER_L_ARROW))
            {
                error_buffer.add(parser_error<CHAR>(phrase_nodes[index]->line,
                                                    phrase_nodes[index]->column,
                                                    error::UNEXPECTED_OPERATOR,
                                                    phrase_nodes[index]->file
                                                    ));

                delete phrase_nodes[index];
                phrase_nodes.remove(index);

                return true;
            }

            return false;
        }

    }
}

#endif // ERROR_OPER_H_INCLUDED
