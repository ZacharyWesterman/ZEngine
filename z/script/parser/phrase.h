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
                INDEX = ident::ID_COUNT,
                INDEXLIST,
                EXPRLIST,
                LIST,
                USER_FUNCCALL,
                BUILTIN_FUNCCALL,
                VARINDEX,
                TYPEVAR,
                VARIABLE,
                OPERAND,
                PARENTHEXPR,
                FACTORIALEXPR,
                NEGATEXPR,
                POWEREXPR,
                MULTIPLYEXPR,
                ADDEXPR,
                BOOLEXPR,
                ASSIGNEXPR,
                GENERALEXPR,

                LX_COUNT
            };
        }



        template <typename CHAR>
        class phrase_t
        {
        public:
            int type;
            int orig_type;

            int line;
            int column;

            error_flag err;

            //optional data
            void* meta;
            double value;


            phrase_t* parent;
            core::array<phrase_t*> children;

            //empty constructor
            phrase_t()
            {
                type = ident::NONE;
                orig_type = ident::NONE;

                line = -1;
                column = -1;

                err = error::NONE;

                meta = NULL;
                value = 0;

                parent = NULL;
            }

            //constructor from ident_t
            phrase_t(const ident_t<CHAR>& token)
            {
                type = phrase::phrase_enum(token.type);
                orig_type = ident::NONE;

                line = token.line;
                column = token.column;

                err = error::NONE;

                meta = token.meta;
                value = token.value;

                parent = NULL;
            }


            //copy data from other phrase, maintain current parent
            const phrase_t& operator=(const phrase_t& other)
            {
                type = other.type;

                line = other.line;
                column = other.column;

                err = other.err;

                meta = other.meta;
                value = other.value;

                children = other.children;

                return *this;
            }


            inline bool operator==(const phrase_t& other) const
            {
                return (type == other.type) && (meta == other.meta);
            }
        };
    }
}

#endif // PHRASE_H_INCLUDED
