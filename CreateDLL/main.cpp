#define MATHLIBRARY_EXPORTS

#include "main.h"

#include "script/operators/add.h"
#include "script/operators/subtract.h"

__declspec(dllexport) void __cdecl get_opers_c(core::array< script::oper::oper_t<char>* >& output)
{
    output.add(new script::oper::add<char>);
    output.add(new script::oper::subtract<char>);
}

__declspec(dllexport) void __cdecl get_opers_w(core::array< script::oper::oper_t<wchar_t>* >& output)
{
    output.add(new script::oper::add<wchar_t>);
    output.add(new script::oper::subtract<wchar_t>);
}
