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

            generic<CPL_CHAR> value;


            phrase_t* parent;
            core::array<phrase_t*> children;

            //keep track of what file this is
            core::string<char>* file;

            //empty constructor
            phrase_t()
            {
                type = ident::NONE;
                orig_type = ident::NONE;

                line = 0;
                column = 0;
                indent = 0;

                meta = NULL;

                parent = NULL;

                file = NULL;
            }

            /*phrase_t(int Type,
                     core::string<CHAR>* File,
                     int Line, int Column, int Indent = 0,
                     void* Meta = NULL)
            {
                type = Type;
                orig_type = ident::NONE;

                line = Line;
                column = Column;
                indent = Indent;

                meta = Meta;

                parent = NULL;
                file = File;
            }*/

            phrase_t(const phrase_t& other,
                     int newType = ident::NONE)
            {
                if (newType != ident::NONE)
                    type = newType;
                else
                    type = other.type;

                orig_type = ident::NONE;

                line = other.line;
                column = other.column;
                indent = other.indent;

                meta = NULL;

                parent = NULL;
                file = other.file;
            }

            //constructor from ident_t
            phrase_t(const ident_t& token)
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
