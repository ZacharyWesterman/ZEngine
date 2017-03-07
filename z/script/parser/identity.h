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
#include <z/script/errors.h>

namespace z
{
    namespace script
    {
        namespace ident
        {
            enum ident_enum
            {
                NONE = 0,

                IDENTIFIER,//1
                OPERATOR,//2

                LPARENTH,//3
                RPARENTH,//4

                LBRACE,//5
                RBRACE,//6

                LBRACKET,//7
                RBRACKET,//8

                COMMA,//9
                SEMICOLON,//10

                ASSIGNMENT,//11

                STRING_LITERAL,//12

                DECIMAL_LITERAL,//13
                BINARY_LITERAL,//14
                HEXADEC_LITERAL,//15
                OCTAL_LITERAL,//16

                FUNCTION,
                COMMAND,

                KEYWORD_MAIN,

                KEYWORD_IF,
                KEYWORD_ELSE,

                KEYWORD_FOR,
                KEYWORD_DO,
                KEYWORD_LOOP,
                KEYWORD_WHILE,

                KEYWORD_GOTO,
                KEYWORD_GOSUB,

                KEYWORD_RUN,
                KEYWORD_INCLUDE,

                KEYWORD_BREAK,
                KEYWORD_RETURN,


                UNKNOWN = -1
            };

        }

        template <typename CHAR>
        class ident_t
        {
        public:
            core::string<CHAR> name;
            ident::ident_enum type;

            int line;
            int column;

            error_flag err;

            void* meta;

            ///constructor with no name, as name may not be needed.
            ident_t (ident::ident_enum t, int lin, int col,
                     error_flag _error = error::NONE)
            {
                type = t;

                line = lin;
                column = col;


                err = _error;

                meta = NULL;
            }

            ///full constructor
            ident_t (core::string<CHAR> s, ident::ident_enum t, int lin, int col,
                     error_flag _error = error::NONE)
            {
                name = s;
                type = t;

                line = lin;
                column = col;

                err = _error;

                meta = NULL;
            }


            ///equality operators
            bool operator==(const ident_t& other) const
            {
                return ((type == other.type) &&
                        (name == other.name));
            }

            bool operator!=(const ident_t& other) const
            {
                return ((type != other.type) &&
                        (name != other.name));
            }

            bool operator>(const ident_t& other) const
            {
                return ((type > other.type) &&
                        (name > other.name));
            }

            bool operator>=(const ident_t& other) const
            {
                return ((type >= other.type) &&
                        (name >= other.name));
            }

            bool operator<(const ident_t& other) const
            {
                return ((type < other.type) &&
                        (name < other.name));
            }

            bool operator<=(const ident_t& other) const
            {
                return ((type <= other.type) &&
                        (name <= other.name));
            }
        };

    }
}

#endif // IDENTITY_H_INCLUDED
