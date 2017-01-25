#define MATHLIBRARY_EXPORTS

#include "main.h"

__declspec(dllexport) script::oper::oper_t<char>* __cdecl get_oper_c()
{
    return new script::oper::add<char>;
}

__declspec(dllexport) script::oper::oper_t<wchar_t>* __cdecl get_oper_w()
{
    return new script::oper::add<wchar_t>;
}
