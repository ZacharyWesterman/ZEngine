#pragma once

#include "z/script/operators/oper_t.h"
#include "z/core/sorted_ref_array.h"

extern "C"
{
    __declspec(dllexport) void __cdecl get_opers_c(z::core::sorted_ref_array< z::script::oper::oper_t<char>* >*);

    __declspec(dllexport) void __cdecl get_opers_w(z::core::sorted_ref_array< z::script::oper::oper_t<wchar_t>* >*);
}

