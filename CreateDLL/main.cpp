#define MATHLIBRARY_EXPORTS

#include "main.h"

#include "script/operators/add.h"

__declspec(dllexport) void __cdecl get_opers_c(core::array< script::oper::oper_t<char> >& output)
{
    output.add(script::oper::add<char>());
}

__declspec(dllexport) void __cdecl get_opers_w(core::array< script::oper::oper_t<wchar_t> >& output)
{
    output.add(script::oper::add<wchar_t>());
}
