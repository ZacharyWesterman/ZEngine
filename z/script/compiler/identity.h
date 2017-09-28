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
 * Last modified:   20 Aug. 2017
**/

#pragma once
#ifndef IDENTITY_H_INCLUDED
#define IDENTITY_H_INCLUDED

#include <z/core/string.h>

#include "../generic.h"
#include "../error.h"

namespace z
{
namespace script
{
    namespace compiler
    {

        enum ident
        {
            NONE = 0,

            LPARENTH,       //1
            RPARENTH,       //2

            LBRACE,         //3
            RBRACE,         //4

            LBRACKET,       //5
            RBRACKET,       //6

            COMMA,          //7
            SEMICOLON,      //8
            PERIOD,         //9

            STRING_LITERAL, //10

            NUMERIC_LITERAL,//11
            COMPLEX_LITERAL,//12

            LITERAL,

            IDENTIFIER,     //13

            KEYWORD,
            OPERATOR,



            UNKNOWN,        //69

            ID_COUNT
        };



        template <typename CHAR>
        class ident_t
        {
        public:
            int type;

            int line;
            int column;
            int indent;

            union
            {
                core::string<CHAR>* meta;
                unsigned long metaValue;
            };

            generic<CHAR> value;

            //keep track of the current file
            core::string<CHAR>* file;

            ///full constructor
            ident_t (ident _type = ident::NONE,
                     int lin = 0, int col = 0, int ind = 0,
                     core::string<CHAR>* symbol_ptr = NULL,
                     core::string<CHAR>* fileID = NULL)
            {
                type = _type;

                line = lin;
                column = col;
                indent = ind;

                meta = symbol_ptr;

                //value = 0;

                file = fileID;
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
}

#endif // IDENTITY_H_INCLUDED
