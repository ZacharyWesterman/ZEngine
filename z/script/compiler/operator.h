#pragma once
#ifndef OPERATOR_H_INCLUDED
#define OPERATOR_H_INCLUDED

#include <z/core/string.h>

namespace z
{
namespace script
{
    namespace compiler
    {
        struct oper
        {
            core::string<char> symbol;
            int value;

            oper(const core::string<char>& Symbol,
                    int Value)
            {
                symbol = Symbol;
                value = Value;
            }

            inline bool operator==(const oper& other) const
            {
                return other.symbol.beginsWith(symbol);
            }
        };
    }
}
}

#endif // OPERATOR_H_INCLUDED
