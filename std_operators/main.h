#pragma once

#include "script/operators/oper_t.h"
#include "core/array.h"

extern "C"
{
    __declspec(dllexport) void __cdecl get_opers_c(core::array< script::oper::oper_t<char>* >&);

    __declspec(dllexport) void __cdecl get_opers_w(core::array< script::oper::oper_t<wchar_t>* >&);
}

