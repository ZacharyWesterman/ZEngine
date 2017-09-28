#pragma once
#ifndef SYNTAXRULE_H_INCLUDED
#define SYNTAXRULE_H_INCLUDED

#include "error.h"

#include "phrase.h"

namespace z
{
namespace script
{
    namespace compiler
    {
        void deleteNode();

        class syntaxRule
        {
        public:
            bool apply();
        };
    }
}
}

#endif // SYNTAXRULE_H_INCLUDED
