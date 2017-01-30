#pragma once

#include "script/operators/oper_t.h"
#include "core/array.h"

extern "C"
{
    __declspec(dllexport) void __cdecl get_funcs_c(core::array< script::func::func_t<char>* >&);

    __declspec(dllexport) void __cdecl get_funcs_w(core::array< script::func::func_t<wchar_t>* >&);
}

