#include "core/loadLibrary.h"

#include <iostream>
using namespace std;

#include "script/operators/oper_t.h"
#include "core/array.h"

bool load_opers(core::array< script::oper::oper_t<char>* >& output, const core::string<char> file_name)
{
    typedef void (__stdcall *_func) (core::array< script::oper::oper_t<char>* >&);

    core::library::lib_t lib = NULL;
    lib = core::library::load(file_name.str());


    if (!lib)
    {
        return false;
    }
    else
    {
        _func get_opers = (_func)core::library::get_function(lib, "get_opers_c");

        if (!get_opers)
        {
            return false;
        }
        else
        {
            get_opers(output);
            return true;
        }
    }
}


bool load_opers(core::array< script::oper::oper_t<wchar_t>* >& output, const core::string<char> file_name)
{
    typedef void (__stdcall *_func) (core::array< script::oper::oper_t<wchar_t>* >&);

    core::library::lib_t lib = NULL;
    lib = core::library::load(file_name.str());


    if (!lib)
    {
        return false;
    }
    else
    {
        _func get_opers = (_func)core::library::get_function(lib, "get_opers_w");

        if (!get_opers)
        {
            return false;
        }
        else
        {
            get_opers(output);
            return true;
        }
    }
}


int main()
{
    core::array< script::oper::oper_t<char>* > opers;

    if(load_opers(opers, "CreateDLL.dll"));

    for (int i=0; i<opers.size(); i++)
    {
        cout << "type[" << opers[i]->str().str() << "]\n";

        core::string<char> a("2"),b("3");

        core::dynamic_stack< core::string<char> > _stack;
        _stack.push(a);
        _stack.push(b);

        script::error_flag err = opers[i]->operate(_stack);

        core::string<char> c;

        _stack.pop(c);

        cout << "operate(" << a.str() << "," << b.str() << ") \nresulted in '";
        cout << c.str() << "' \nand returned error{" << err << "}.\n\n";
    }


    return 0;
}
