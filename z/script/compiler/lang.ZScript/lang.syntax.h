#pragma once
#ifndef LANG_SYNTAX_H_INCLUDED
#define LANG_SYNTAX_H_INCLUDED

#include <z/core/array.h>
#include <z/script/compiler/syntaxRule.h>
//#include <z/zcript/compiler/identEnum.h>

enum syntax
{
    IDENTIFIERLIST = z::script::compiler::ident::ID_COUNT,
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

#include "lang.syntax/program.h"

#include "lang.syntax/operand.h"
#include "lang.syntax/variable.h"
#include "lang.syntax/parenthexpr.h"
#include "lang.syntax/factorialexpr.h"
#include "lang.syntax/add1expr.h"
#include "lang.syntax/negatexpr.h"
#include "lang.syntax/powerexpr.h"
#include "lang.syntax/multiplyexpr.h"
#include "lang.syntax/addexpr.h"
#include "lang.syntax/boolexpr.h"
#include "lang.syntax/assignexpr.h"


#define SYN_RULE z::script::compiler::syntaxRule
#define ARRAY z::core::array<SYN_RULE*>


ARRAY* genSyntaxRules()
{
    ARRAY* rules = new ARRAY
    {
        new z::script::compiler::operand,
        new z::script::compiler::variable,

        new z::script::compiler::parenthexpr,
        new z::script::compiler::factorialexpr,
        new z::script::compiler::add1expr,
        new z::script::compiler::negatexpr,
        new z::script::compiler::powerexpr,
        new z::script::compiler::multiplyexpr,
        new z::script::compiler::addexpr,
        new z::script::compiler::boolexpr,
        new z::script::compiler::assignexpr,
    };

    return rules;
}

SYN_RULE* genProgramRule()
{
    return new z::script::compiler::program;
}

#undef SYN_RULE
#undef ARRAY

#endif // LANG_SYNTAX_H_INCLUDED
