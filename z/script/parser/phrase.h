/**
 * File:            phrase.h
 * Namespace:       z::script
 * Description:     Lexer phrase template. This class is used by
 *                  the lexer to organize tokens into valid
 *                  'phrases' (e.g. expressions, commands, etc.).
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   14 May 2017
**/


#pragma once
#ifndef PHRASE_H_INCLUDED
#define PHRASE_H_INCLUDED

#include <z/core/array.h>

#include "identity.h"

namespace z
{
    namespace script
    {
        namespace phrase
        {
            enum phrase_enum
            {
                NONE = 0,

                LITERAL,

                LIST,
            };
        }

        template <typename CHAR>
        class phrase_t
        {
        public:
            phrase::phrase_enum type;

            int line_begin;
            int column_begin;

            int line_end;
            int column_end;

            error_flag err;

            ident_t<CHAR> data;

            core::array<phrase_t> children;

            phrase_t(phrase::phrase_enum p_type = phrase::NONE)
            {
                type = p_type;

                line_begin = -1;
                column_begin = -1;

                line_end = -1;
                column_end = -1;

                err = error::NONE;
            }

            inline bool operator==(const phrase_t& other) const
            {
                return (type == other.type) && (data == other.data);
            }
        };
    }
}

#endif // PHRASE_H_INCLUDED
