#pragma once

#include "z/script/operators/oper_t.h"
#include "z/core/array.h"

extern "C"
{
    __declspec(dllexport) void __cdecl get_opers_c(z::core::array< z::script::oper::oper_t<char>* >*);

    __declspec(dllexport) void __cdecl get_opers_w(z::core::array< z::script::oper::oper_t<wchar_t>* >*);
}

