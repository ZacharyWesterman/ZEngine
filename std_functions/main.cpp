#define MATHLIBRARY_EXPORTS

#include "main.h"

#include "script/functions/func_t.h"


__declspec(dllexport) void __cdecl get_funcs_c(core::array< script::func::func_t<char>* >& output)
{
    //Default function
    output.add(new script::func::func_t<char>);
}

__declspec(dllexport) void __cdecl get_funcs_w(core::array< script::oper::oper_t<wchar_t>* >& output)
{
    //Default function
    output.add(new script::func::func_t<wchar_t>);
}
