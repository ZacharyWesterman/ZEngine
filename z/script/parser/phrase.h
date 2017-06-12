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
 * Last modified:   12 Jun. 2017
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
                IDENTIFIERLIST = ident::ID_COUNT,
                COMMAND,

                STATEMENTLIST,
                STATEMENT,
                IF_STATEMENT,
                FOR_STATEMENT,
                FOREACH_STATEMENT,
                LOOP_STATEMENT,
                WHILE_PRE_STMT,
                WHILE_POST_STMT,
                RUN_STATEMENT,
                RETURN_STATEMENT,
                WAIT_STATEMENT,
                UNTIL_STATEMENT,
                LABEL_STATEMENT,
                GOTO_STATEMENT,
                GOSUB_STATEMENT,

                SUBROUTINE_DECL,

                VARIABLE_DECL,
                TYPEVAR_DECL,

                INDEX,
                INDEXLIST,

                EXPRLIST,
                LIST,

                FUNCCALL,
                TYPE_FUNCCALL,

                VARINDEX,
                TYPEVAR,
                VARIABLE,
                OPERAND,

                PARENTHEXPR,
                FACTORIALEXPR,
                ADD1EXPR,
                NEGATEXPR,
                POWEREXPR,
                MULTIPLYEXPR,
                ADDEXPR,
                BOOLEXPR,
                ASSIGNEXPR,
                DIMENSIONEXPR,
                SIZEOFEXPR,

                GLOBALDECL,
                EXTERNALDECL,
                SHAREDDECL,

                FORMALVARDECL,
                FORMALTYPEDECL,
                FORMALDECLLIST,

                FUNCTION_DECL,
                TYPEDECL,

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
