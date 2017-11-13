#pragma once
#ifndef SEMANTICRULE_H_INCLUDED
#define SEMANTICRULE_H_INCLUDED

#include "../error.h"

#include "signatures.h"
#include "varScope.h"

namespace z
{
namespace script
{
    namespace compiler
    {
        class semanticRule
        {
            virtual ~semanticRule() {}

            virtual check(varScope*,
                          core::array<error>*) = 0;
        };
    }
}
}

#endif // SEMANTICRULE_H_INCLUDED
