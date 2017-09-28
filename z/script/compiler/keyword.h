#pragma once
#ifndef KEYWORD_H_INCLUDED
#define KEYWORD_H_INCLUDED

#include <z/core/string.h>

namespace z
{
namespace script
{
    namespace compiler
    {
        struct keyword
        {
            core::string<char> symbol;
            int value;

            keyword(const core::string<char>& Symbol,
                    int Value)
            {
                symbol = Symbol;
                value = Value;
            }

            inline bool operator==(const keyword& other) const
            {
                return symbol == other.symbol;
            }
        };
    }
}
}

#endif // KEYWORD_H_INCLUDED
