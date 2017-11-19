#pragma once
#ifndef LANG_SEMANTICS_H_INCLUDED
#define LANG_SEMANTICS_H_INCLUDED

#include <z/core/array.h>
#include <z/script/compiler/semanticRule.h>


///include semantic rules here
#include "lang.semantics/variable_decl_sem.h"
#include "lang.semantics/typevar_decl_sem.h"
#include "lang.semantics/typedecl_sem.h"


#define SEM_RULE z::script::compiler::semanticRule
#define ARRAY z::core::array< SEM_RULE* >

ARRAY* genSemanticRules()
{
    ARRAY* rules = new ARRAY
    {
        new z::script::compiler::variable_decl_sem,
        new z::script::compiler::typevar_decl_sem,
        new z::script::compiler::typedecl_sem,
    };

    return rules;
}

#undef ARRAY
#undef SEM_RULE

#endif // LANG_SEMANTICS_H_INCLUDED
