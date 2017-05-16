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

            int ident_begin;
            int ident_end;

            core::array<phrase_t> children;

            phrase_t()
            {
                ident_begin = -1;
                ident_end = -1;

                type = phrase::NONE;
            }
        };
    }
}

#endif // PHRASE_H_INCLUDED
