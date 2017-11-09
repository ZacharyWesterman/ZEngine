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

#include "lang.syntax/variable_decl.h"
#include "lang.syntax/typevar_decl.h"

#include "lang.syntax/parenthexpr.h"
#include "lang.syntax/factorialexpr.h"
#include "lang.syntax/add1expr.h"
#include "lang.syntax/negatexpr.h"
#include "lang.syntax/powerexpr.h"
#include "lang.syntax/multiplyexpr.h"
#include "lang.syntax/addexpr.h"
#include "lang.syntax/boolexpr.h"
#include "lang.syntax/assignexpr.h"
#include "lang.syntax/sizeofexpr.h"
#include "lang.syntax/dimensionexpr.h"

#include "lang.syntax/range.h"
#include "lang.syntax/rangelist.h"
#include "lang.syntax/index.h"
#include "lang.syntax/varindex.h"

#include "lang.syntax/exprlist.h"
#include "lang.syntax/list.h"

#include "lang.syntax/statement.h"
#include "lang.syntax/statementlist.h"
#include "lang.syntax/for_statement.h"
#include "lang.syntax/return_statement.h"

#include "lang.syntax/identifierlist.h"
#include "lang.syntax/command.h"

#include "lang.syntax/funccall.h"
#include "lang.syntax/func_prototype.h"
#include "lang.syntax/function_decl.h"
#include "lang.syntax/formalvardecl.h"
#include "lang.syntax/formaltypedecl.h"
#include "lang.syntax/formaldecllist.h"

#include "lang.syntax/type_funccall.h"
#include "lang.syntax/typevar.h"


#define SYN_RULE z::script::compiler::syntaxRule
#define ARRAY z::core::array<SYN_RULE*>


ARRAY* genSyntaxRules()
{
    ARRAY* rules = new ARRAY
    {
        new z::script::compiler::operand,
        new z::script::compiler::variable,

        new z::script::compiler::variable_decl,
        new z::script::compiler::typevar_decl,

        new z::script::compiler::parenthexpr,
        new z::script::compiler::factorialexpr,
        new z::script::compiler::add1expr,
        new z::script::compiler::negatexpr,
        new z::script::compiler::powerexpr,
        new z::script::compiler::multiplyexpr,
        new z::script::compiler::addexpr,
        new z::script::compiler::boolexpr,
        new z::script::compiler::assignexpr,
        new z::script::compiler::sizeofexpr,
        new z::script::compiler::dimensionexpr,

        new z::script::compiler::_range,
        new z::script::compiler::rangelist,
        new z::script::compiler::_index,
        new z::script::compiler::varindex,

        new z::script::compiler::exprlist,
        new z::script::compiler::_list,

        new z::script::compiler::statement,
        new z::script::compiler::statementlist,
        new z::script::compiler::for_statement,
        new z::script::compiler::return_statement,

        new z::script::compiler::identifierlist,
        new z::script::compiler::_command,

        new z::script::compiler::funccall,
        new z::script::compiler::func_prototype,
        new z::script::compiler::function_decl,
        new z::script::compiler::formalvardecl,
        new z::script::compiler::formaltypedecl,
        new z::script::compiler::formaldecllist,

        new z::script::compiler::type_funccall,
        new z::script::compiler::typevar,
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
