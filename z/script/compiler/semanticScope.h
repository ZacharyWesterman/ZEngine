#pragma once
#ifndef SEMANTICSCOPE_H_INCLUDED
#define SEMANTICSCOPE_H_INCLUDED

#include <z/core/array.h>
#include "varScope.h"
#include "signatures.h"

#include "phrase.h"


namespace z
{
namespace script
{
    namespace compiler
    {
        struct semanticScope
        {
            //Variables
            varScope globalScope;
            varScope* currentScope;

            //Functions
            core::array<funcSignature> functionList;
            core::array< void* > paramList;

            //Type declarations
            core::array<typeSignature> typeList;
            core::array<symID> typeFuncList;
            core::array<symID> typeVarList;

            //Expression checking
            core::array<void*> exprTypeList;
            phrase_t* exprStart;
            void* currentType;

            symID uniqueID_Current;


            semanticScope();
            void clear();
        };

        semanticScope::semanticScope()
        {
            globalScope.parent = NULL;
            currentScope = &globalScope;

            uniqueID_Current = 1;

            exprStart = NULL;
            currentType = NULL;
        }

        void semanticScope::clear()
        {
            globalScope.parent = NULL;
            currentScope = &globalScope;

            uniqueID_Current = 1;

            exprStart = NULL;
            currentType = NULL;

            functionList.clear();
            paramList.clear();

            typeList.clear();
            typeFuncList.clear();
            typeVarList.clear();

            exprTypeList.clear();
        }
    }
}
}

#endif // SEMANTICSCOPE_H_INCLUDED
