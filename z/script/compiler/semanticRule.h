#pragma once
#ifndef SEMANTICRULE_H_INCLUDED
#define SEMANTICRULE_H_INCLUDED

#include "../error.h"

#include "signatures.h"
#include "semanticScope.h"
#include "phrase.h"

#include "../command.h"
#include "../function.h"

namespace z
{
namespace script
{
    namespace compiler
    {
        class semanticRule
        {
            virtual ~semanticRule() {}

            //return TRUE if node is of correct type
            //(don't check every rule if we found the right one)
            virtual bool check(const core::array< command* >*, //list of commands
                               const core::array< function* >*, //list of functions
                               semanticScope*,
                               phrase_t*, //current node
                               int&,//next node index to enter (
                               core::array<error>*) = 0;


            void enter_scope(varScope* scope)
            {
                scope->children.add(varScope(scope));
                scope = &(scope->children[scope->children.size()-1]);
            }

            void exit_scope(varScope* scope)
            {
                if (scope->parent)
                    scope = scope->parent;
            }
        };
    }
}
}

#endif // SEMANTICRULE_H_INCLUDED
