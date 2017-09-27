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
        enum phrase
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
            STOP_STATEMENT,
            RETURN_STATEMENT,
            WAIT_STATEMENT,
            UNTIL_STATEMENT,
            LABEL_STATEMENT,
            GOTO_STATEMENT,
            GOSUB_STATEMENT,

            SUBROUTINE_DECL,

            VARIABLE_DECL,
            TYPEVAR_DECL,

            INT_DECLLIST,
            TYPEDECL,

            EXTERNALDECL,
            SHAREDDECL,

            FUNC_PROTOTYPE,
            FUNCTION_DECL,

            RANGE,
            RANGELIST,
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

            FORMALVARDECL,
            FORMALTYPEDECL,
            FORMALDECLLIST,

            PROGRAM,

            FUNCCALL_BUILTIN,

            LX_COUNT
        };



        template <typename CHAR>
        class phrase_t
        {
        public:
            int type;
            int orig_type;

            int line;
            int column;

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

                line = -1;
                column = -1;

                meta = NULL;
                value = 0;

                parent = NULL;

                file = NULL;
            }

            //constructor from ident_t
            phrase_t(const ident_t<CHAR>& token)
            {
                type = phrase(token.type);
                orig_type = ident::NONE;

                line = token.line;
                column = token.column;

                meta = token.meta;
                value = token.value;

                parent = NULL;

                file = token.file;
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
}

#endif // PHRASE_H_INCLUDED
