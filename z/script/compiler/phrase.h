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
 * Last modified:   22 Aug. 2017
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
    namespace compiler
    {

        template <typename CHAR>
        class phrase_t
        {
        public:
            int type;
            int orig_type;

            int line;
            int column;
            int indent;

            //optional data
            union
            {
                void* meta;
                unsigned long metaValue;
            };

            generic<CHAR> value;


            phrase_t* parent;
            core::array<phrase_t*> children;

            //keep track of what file this is
            core::string<CHAR>* file;

            //empty constructor
            phrase_t()
            {
                type = ident::NONE;
                orig_type = ident::NONE;

                line = 0;
                column = 0;
                indent = 0;

                meta = NULL;
                value = 0;

                parent = NULL;

                file = NULL;
            }

            //constructor from ident_t
            phrase_t(const ident_t<CHAR>& token)
            {
                type = token.type;
                orig_type = ident::NONE;

                line = token.line;
                column = token.column;
                indent = token.indent;

                meta = token.meta;
                value = token.value;

                parent = NULL;

                file = token.file;
            }


            //copy data from other phrase, maintain current parent
            const phrase_t& operator=(const phrase_t& other)
            {
                type = other.type;
                orig_type = other.orig_type;

                line = other.line;
                column = other.column;
                indent = other.indent;

                meta = other.meta;
                value = other.value;

                file = other.file;

                return *this;
            }


            inline bool operator==(const phrase_t& other) const
            {
                return (type == other.type) && (meta == other.meta);
            }
        };
    }
}
}

#endif // PHRASE_H_INCLUDED
