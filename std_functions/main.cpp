#define MATHLIBRARY_EXPORTS

#include "main.h"

#include "script/operators/add.h"
#include "script/operators/subtract.h"
#include "script/operators/negate.h"

#include "script/operators/multiply.h"
#include "script/operators/divide.h"
#include "script/operators/integer_divide.h"
#include "script/operators/modulus.h"

#include "script/operators/power.h"
#include "script/operators/factorial.h"

#include "script/operators/equals.h"
#include "script/operators/not_equal.h"
#include "script/operators/greater_than.h"
#include "script/operators/less_than.h"
#include "script/operators/greater_than_equal.h"
#include "script/operators/less_than_equal.h"

#include "script/operators/and.h"
#include "script/operators/or.h"
#include "script/operators/nand.h"
#include "script/operators/nor.h"
#include "script/operators/xor.h"
#include "script/operators/xnor.h"
#include "script/operators/not.h"


__declspec(dllexport) void __cdecl get_opers_c(core::array< script::oper::oper_t<char>* >& output)
{
    //Addition
    output.add(new script::oper::add<char>);
    output.add(new script::oper::subtract<char>);
    output.add(new script::oper::negate<char>);

    //Multiplication
    output.add(new script::oper::multiply<char>);
    output.add(new script::oper::divide<char>);
    output.add(new script::oper::integer_divide<char>);
    output.add(new script::oper::modulus<char>);

    //Advanced operations
    output.add(new script::oper::power<char>);
    output.add(new script::oper::factorial<char>);

    //Equality
    output.add(new script::oper::equals<char>);
    output.add(new script::oper::not_equal<char>);
    output.add(new script::oper::greater_than<char>);
    output.add(new script::oper::less_than<char>);
    output.add(new script::oper::greater_than_equal<char>);
    output.add(new script::oper::less_than_equal<char>);

    //Boolean logic
    output.add(new script::oper::_and<char>);
    output.add(new script::oper::_or<char>);
    output.add(new script::oper::_nand<char>);
    output.add(new script::oper::_nor<char>);
    output.add(new script::oper::_xor<char>);
    output.add(new script::oper::_xnor<char>);
    output.add(new script::oper::_not<char>);
}

__declspec(dllexport) void __cdecl get_opers_w(core::array< script::oper::oper_t<wchar_t>* >& output)
{
    //Addition
    output.add(new script::oper::add<wchar_t>);
    output.add(new script::oper::subtract<wchar_t>);
    output.add(new script::oper::negate<wchar_t>);

    //Multiplication
    output.add(new script::oper::multiply<wchar_t>);
    output.add(new script::oper::divide<wchar_t>);
    output.add(new script::oper::integer_divide<wchar_t>);
    output.add(new script::oper::modulus<wchar_t>);

    //Advanced operations
    output.add(new script::oper::power<wchar_t>);
    output.add(new script::oper::factorial<wchar_t>);

    //Equality
    output.add(new script::oper::equals<wchar_t>);
    output.add(new script::oper::not_equal<wchar_t>);
    output.add(new script::oper::greater_than<wchar_t>);
    output.add(new script::oper::less_than<wchar_t>);
    output.add(new script::oper::greater_than_equal<wchar_t>);
    output.add(new script::oper::less_than_equal<wchar_t>);

    //Boolean logic
    output.add(new script::oper::_and<wchar_t>);
    output.add(new script::oper::_or<wchar_t>);
    output.add(new script::oper::_nand<wchar_t>);
    output.add(new script::oper::_nor<wchar_t>);
    output.add(new script::oper::_xor<wchar_t>);
    output.add(new script::oper::_xnor<wchar_t>);
    output.add(new script::oper::_not<wchar_t>);
}
