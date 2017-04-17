/**
 * File:            identity.h
 * Namespace:       z::script
 * Description:     Template for script identities. An example
 *                  identifier could be represented as <"+",OPERATOR>.
 *                  For error reporting purposes, all identities include
 *                  the line and column of their first character from
 *                  the input stream.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   4 Mar. 2017
**/

#pragma once
#ifndef IDENTITY_H_INCLUDED
#define IDENTITY_H_INCLUDED

#include <z/core/string.h>
#include "../errors.h"

namespace z
{
    namespace script
    {
        namespace ident
        {
            enum ident_enum
            {
                NONE = 0,

                LPARENTH,//1
                RPARENTH,//2

                LBRACE,//3
                RBRACE,//4

                LBRACKET,//5
                RBRACKET,//6

                COMMA,//7
                SEMICOLON,//8
                PERIOD,

                ASSIGNMENT,//9

                STRING_LITERAL,//10

                NUMERIC_LITERAL,

                IDENTIFIER,//15
                OPERATOR,//16

                FUNCTION,//17
                COMMAND,//18

                KEYWORD_MAIN,//19

                KEYWORD_IF,//20
                KEYWORD_ELSE,//21

                KEYWORD_FOR,//22
                KEYWORD_DO,//23
                KEYWORD_LOOP,//24
                KEYWORD_WHILE,//25

                KEYWORD_GOTO,//26
                KEYWORD_GOSUB,//27

                KEYWORD_RUN,//28
                KEYWORD_INCLUDE,//29

                KEYWORD_BREAK,//30
                KEYWORD_RETURN,//31

                KEYWORD_EXIT, //32

                UNKNOWN = -1
            };

        }

        template <typename CHAR>
        class ident_t
        {
        public:
            ident::ident_enum type;

            int line;
            int column;

            error_flag err;

            core::string<CHAR>* meta;

            double value;


            ///full constructor
            ident_t (ident::ident_enum t, int lin, int col,
                     core::string<CHAR>* symbol_ptr = NULL, error_flag _error = error::NONE)
            {
                type = t;

                line = lin;
                column = col;

                err = _error;

                meta = symbol_ptr;

                value = 0;
            }


            ///equality operators
            inline bool operator==(const ident_t& other) const
            {
                return (type == other.type);
            }

            inline bool operator!=(const ident_t& other) const
            {
                return (type != other.type);
            }

            inline bool operator>(const ident_t& other) const
            {
                return (type > other.type);
            }

            inline bool operator>=(const ident_t& other) const
            {
                return (type >= other.type);
            }

            inline bool operator<(const ident_t& other) const
            {
                return (type < other.type);
            }

            inline bool operator<=(const ident_t& other) const
            {
                return (type <= other.type);
            }
        };

    }
}

#endif // IDENTITY_H_INCLUDED
