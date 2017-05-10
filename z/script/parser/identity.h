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
                PERIOD,//9

                ASSIGNMENT,//10

                STRING_LITERAL,//11

                NUMERIC_LITERAL,//12

                IDENTIFIER,//13
                OPERATOR,//14

                FUNCTION,//15
                COMMAND,//16

                KEYWORD_MAIN,//17

                KEYWORD_IF,//18
                KEYWORD_ELSE,//19

                KEYWORD_FOR,//20
                KEYWORD_DO,//21
                KEYWORD_LOOP,//22
                KEYWORD_WHILE,//23

                KEYWORD_GOTO,//24
                KEYWORD_GOSUB,//25

                KEYWORD_RUN,//26
                KEYWORD_INCLUDE,//27

                KEYWORD_BREAK,//28
                KEYWORD_RETURN,//29

                KEYWORD_EXIT, //30

                KEYWORD_WAIT,//31

                KEYWORD_TYPE,//32

                KEYWORD_GLOBAL,//33
                KEYWORD_EXTERNAL,//34

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
            ident_t (ident::ident_enum t = ident::NONE, int lin = 0, int col = 0,
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
