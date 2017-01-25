#define MATHLIBRARY_EXPORTS

#include "main.h"

#include "script/operators/add.h"
#include "script/operators/subtract.h"

#include "script/operators/multiply.h"
#include "script/operators/divide.h"
#include "script/operators/integer_divide.h"
#include "script/operators/modulus.h"

#include "script/operators/power.h"
#include "script/operators/factorial.h"


__declspec(dllexport) void __cdecl get_opers_c(core::array< script::oper::oper_t<char>* >& output)
{
    output.add(new script::oper::add<char>);
    output.add(new script::oper::subtract<char>);

    output.add(new script::oper::multiply<char>);
    output.add(new script::oper::divide<char>);
    output.add(new script::oper::integer_divide<char>);
    output.add(new script::oper::modulus<char>);

    output.add(new script::oper::power<char>);
    output.add(new script::oper::factorial<char>);
}

__declspec(dllexport) void __cdecl get_opers_w(core::array< script::oper::oper_t<wchar_t>* >& output)
{
    output.add(new script::oper::add<wchar_t>);
    output.add(new script::oper::subtract<wchar_t>);

    output.add(new script::oper::multiply<wchar_t>);
    output.add(new script::oper::divide<wchar_t>);
    output.add(new script::oper::integer_divide<wchar_t>);
    output.add(new script::oper::modulus<wchar_t>);

    output.add(new script::oper::power<wchar_t>);
    output.add(new script::oper::factorial<wchar_t>);
}
