#pragma once
#ifndef IS_SCRIPT_STRING_H_INCLUDED
#define IS_SCRIPT_STRING_H_INCLUDED

#include "../core/string.h"

namespace script
{
    template <typename CHAR>
    bool is_script_string(const core::string<CHAR>& text)
    {
        core::string<CHAR> quote = (CHAR)34;

        return text.beginsWith(quote);
    }
}

#endif // IS_SCRIPT_STRING_H_INCLUDED
