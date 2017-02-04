#define MATHLIBRARY_EXPORTS

#include "main.h"

#include "z/script/operators/add.h"
#include "z/script/operators/subtract.h"
#include "z/script/operators/negate.h"

#include "z/script/operators/multiply.h"
#include "z/script/operators/divide.h"
#include "z/script/operators/integer_divide.h"
#include "z/script/operators/modulus.h"

#include "z/script/operators/power.h"
#include "z/script/operators/factorial.h"

#include "z/script/operators/equals.h"
#include "z/script/operators/not_equal.h"
#include "z/script/operators/greater_than.h"
#include "z/script/operators/less_than.h"
#include "z/script/operators/greater_than_equal.h"
#include "z/script/operators/less_than_equal.h"

#include "z/script/operators/and.h"
#include "z/script/operators/or.h"
#include "z/script/operators/nand.h"
#include "z/script/operators/nor.h"
#include "z/script/operators/xor.h"
#include "z/script/operators/xnor.h"
#include "z/script/operators/not.h"


__declspec(dllexport) void __cdecl get_opers_c(z::core::array< z::script::oper::oper_t<char>* >& output)
{
    //Addition
    output.add(new z::script::oper::add<char>);
    output.add(new z::script::oper::subtract<char>);
    output.add(new z::script::oper::negate<char>);

    //Multiplication
    output.add(new z::script::oper::multiply<char>);
    output.add(new z::script::oper::divide<char>);
    output.add(new z::script::oper::integer_divide<char>);
    output.add(new z::script::oper::modulus<char>);

    //Advanced operations
    output.add(new z::script::oper::power<char>);
    output.add(new z::script::oper::factorial<char>);

    //Equality
    output.add(new z::script::oper::equals<char>);
    output.add(new z::script::oper::not_equal<char>);
    output.add(new z::script::oper::greater_than<char>);
    output.add(new z::script::oper::less_than<char>);
    output.add(new z::script::oper::greater_than_equal<char>);
    output.add(new z::script::oper::less_than_equal<char>);

    //Boolean logic
    output.add(new z::script::oper::_and<char>);
    output.add(new z::script::oper::_or<char>);
    output.add(new z::script::oper::_nand<char>);
    output.add(new z::script::oper::_nor<char>);
    output.add(new z::script::oper::_xor<char>);
    output.add(new z::script::oper::_xnor<char>);
    output.add(new z::script::oper::_not<char>);
}

__declspec(dllexport) void __cdecl get_opers_w(z::core::array< z::script::oper::oper_t<wchar_t>* >& output)
{
    //Addition
    output.add(new z::script::oper::add<wchar_t>);
    output.add(new z::script::oper::subtract<wchar_t>);
    output.add(new z::script::oper::negate<wchar_t>);

    //Multiplication
    output.add(new z::script::oper::multiply<wchar_t>);
    output.add(new z::script::oper::divide<wchar_t>);
    output.add(new z::script::oper::integer_divide<wchar_t>);
    output.add(new z::script::oper::modulus<wchar_t>);

    //Advanced operations
    output.add(new z::script::oper::power<wchar_t>);
    output.add(new z::script::oper::factorial<wchar_t>);

    //Equality
    output.add(new z::script::oper::equals<wchar_t>);
    output.add(new z::script::oper::not_equal<wchar_t>);
    output.add(new z::script::oper::greater_than<wchar_t>);
    output.add(new z::script::oper::less_than<wchar_t>);
    output.add(new z::script::oper::greater_than_equal<wchar_t>);
    output.add(new z::script::oper::less_than_equal<wchar_t>);

    //Boolean logic
    output.add(new z::script::oper::_and<wchar_t>);
    output.add(new z::script::oper::_or<wchar_t>);
    output.add(new z::script::oper::_nand<wchar_t>);
    output.add(new z::script::oper::_nor<wchar_t>);
    output.add(new z::script::oper::_xor<wchar_t>);
    output.add(new z::script::oper::_xnor<wchar_t>);
    output.add(new z::script::oper::_not<wchar_t>);
}
