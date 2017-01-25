#include "core/loadLibrary.h"

#include <iostream>
using namespace std;

#include "script/operators/oper_t.h"

bool load_oper(script::oper::oper_t<char>*& output, const core::string<char> file_name)
{
    typedef script::oper::oper_t<char>* (__stdcall *_func) ();

    core::library::lib_t lib = NULL;
    lib = core::library::load(file_name.str());


    if (!lib)
    {
        return false;
    }
    else
    {
        _func get_oper = (_func)core::library::get_function(lib, "get_oper_c");

        if (!get_oper)
        {
            return false;
        }
        else
        {
            output = get_oper();
            return true;
        }
    }
}


bool load_oper(script::oper::oper_t<wchar_t>*& output, const core::string<char> file_name)
{
    typedef script::oper::oper_t<wchar_t>* (__stdcall *_func) ();

    core::library::lib_t lib = NULL;
    lib = core::library::load(file_name.str());


    if (!lib)
    {
        return false;
    }
    else
    {
        _func get_oper = (_func)core::library::get_function(lib, "get_oper_c");

        if (!get_oper)
        {
            return false;
        }
        else
        {
            output = get_oper();
            return true;
        }
    }
}


int main()
{
    script::oper::oper_t<char>* Operator = NULL;

    if(load_oper(Operator, "CreateDLL.dll"));


    cout << "type[" << Operator->str().str() << "]\n\n";

    core::string<char> a("2"),b("3");

    core::dynamic_stack< core::string<char> > _stack;
    _stack.push(a);
    _stack.push(b);

    script::error_flag err = Operator->operate(_stack);

    core::string<char> c;

    _stack.pop(c);

    cout << "operate(" << a.str() << "," << b.str() << ") \nresulted in '";
    cout << c.str() << "' \nand returned error{" << err << "}.\n";



    return 0;
}
