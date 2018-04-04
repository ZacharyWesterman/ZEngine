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

#include "identEnum.h"

#include "../generic.h"
#include "../error.h"

#ifndef CPL_CHAR
#define CPL_CHAR wchar_t
#endif // CPL_CHAR

namespace z
{
namespace script
{
    namespace compiler
    {

        class ident_t
        {
        public:
            int type;

            int line;
            int column;
            int indent;

            union
            {
                void* meta;
                unsigned long metaValue;
            };

            generic<CPL_CHAR> value;

            //keep track of the current file
            core::string<char>* file;

            ///full constructor
            ident_t (ident _type = ident::NONE,
                     int lin = 0, int col = 0, int ind = 0,
                     void* symbol_ptr = NULL,
                     core::string<char>* fileID = NULL)
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
