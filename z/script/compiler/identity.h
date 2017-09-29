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

namespace z
{
namespace script
{
    namespace compiler
    {

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
