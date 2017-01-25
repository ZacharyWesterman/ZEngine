#pragma once

#include "script/operators/add.h"

extern "C"
{
    __declspec(dllexport) script::oper::oper_t<char>* __cdecl get_oper_c();

    __declspec(dllexport) script::oper::oper_t<wchar_t>* __cdecl get_oper_w();
}

