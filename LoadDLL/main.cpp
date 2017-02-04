#include "z/core/loadLibrary.h"

#include <windows.h>

#include <iostream>
using namespace std;

#include "z/script/operators/oper_t.h"
#include "z/core/array.h"

#include "z/file/list_files_in_dir.h"

using namespace z;

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
    //get list of all dlls in the directory
    core::array< core::string<char> > files;

    core::string<char> dir = "operators";
    core::string<char> type = "dll";

    file::list_files_in_dir(dir, type, files);

    cout << "Found " << files.size();
    cout << ((files.size() == 1) ? (" library in dir '") : (" libraries in dir '"));
    cout << dir.str() << "' of type '" << type.str() << "':\n\n";

    for (int i=0; i<files.size(); i++)
    {
        cout << files[i].str() << endl;
    }

    cout << "\n-----------------\n\n";


    //load the dlls
    core::array< script::oper::oper_t<char>* > opers;

    for (int lib=0; lib<files.size(); lib++)
    {
        core::string<char> file_dir = dir + "/";
        file_dir += files[lib];

        load_opers(opers, file_dir);

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

            cout << "operate(";
            opers[i]->binary() ? (cout <<a.str() << "," << b.str()) : (cout << b.str());
            cout << ") \nresulted in '";
            cout << c.str() << "' \nand returned error{" << err << "}.\n\n";
        }
    }

    system("PAUSE");

    return 0;
}
