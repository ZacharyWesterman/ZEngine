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
 * Last modified:   15 Feb. 2017
**/

#pragma once
#ifndef IDENTITY_H_INCLUDED
#define IDENTITY_H_INCLUDED

#include "../core/string.h"
#include "script_errors.h"

namespace z
{
    namespace script
    {
        namespace ident
        {
            enum ident_enum
            {
                NONE = 0,

                IDENTIFIER,
                OPERATOR,

                LPARENTH,
                RPARENTH,

                LBRACE,
                RBRACE,

                LBRACKET,
                RBRACKET,

                COMMA,
                SEMICOLON,

                NUMBER_LITERAL,
                STRING_LITERAL,

                VARIABLE,
                VARTYPE,
                SUBVARIABLE,

                FUNCTION,
                COMMAND,
                UNKNOWN
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
            ident_t (ident::ident_enum t, int lin, int col)
            {
                type = t;

                line = lin;
                column = col;


                err = error::NONE;

                meta = NULL;
            }

            ///full constructor
            ident_t (core::string<CHAR> s, ident::ident_enum t, int lin, int col)
            {
                name = s;
                type = t;

                line = lin;
                column = col;

                err = error::NONE;

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
