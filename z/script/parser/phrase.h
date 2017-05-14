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
        template <typename CHAR>
        class phrase
        {
        public:
            ident_t<CHAR> data;

            core::array<phrase> children;
        };
    }
}

#endif // PHRASE_H_INCLUDED
