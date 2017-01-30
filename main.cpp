//#include "engine/game.h"

//#include "core/sorted_array.h"

#include "core/stringUtils/char_is_alphanumeric.h"
#include "core/stringUtils/char_is_whiteSpace.h"

#include "script/preParser.h"

#include "core/loadLibrary.h"

#include <windows.h>

#include <iostream>
using namespace std;

#include "script/operators/oper_t.h"
#include "core/array.h"

#include "file/list_files_in_dir.h"

bool load_opers(core::array< script::oper::oper_t<char>* >& output, const core::string<char>& file_name)
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


bool load_opers(core::array< script::oper::oper_t<wchar_t>* >& output, const core::string<char>& file_name)
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


void load_all_opers(core::array< script::oper::oper_t<char>* >& output,
                    const core::string<char>& folder_name)
{
    core::array< core::string<char> > files;

    file::list_files_in_dir(folder_name, "dll", files);

    for(int i=0; i<files.size(); i++)
    {
        core::string<char> file_name = folder_name;
        file_name += '/';
        file_name += files[i];

        load_opers(output, file_name);
    }
}


int main()
{
    //load operators
    core::array< script::oper::oper_t<char>* > operators;

    load_all_opers(operators, "operators");



    //test the pre-parser
    script::preParser<char> preP (operators);

    cout << "err[" << preP.split("print 10{\"arr[10]=\",arr[10]}") << "]\n";

    for (int i=0; i<preP.arr.size(); i++)
    {
        cout << preP.arr[i].str.str() << endl;
    }




    cout << "\nDone.\n";

    cout << endl;
    cout << "( " << (int)'(' << ':' << (int)L'(' << endl;
    cout << ") " << (int)')' << ':' << (int)L')' << endl;
    cout << "[ " << (int)'[' << ':' << (int)L'[' << endl;
    cout << "] " << (int)']' << ':' << (int)L']' << endl;
    cout << "{ " << (int)'{' << ':' << (int)L'{' << endl;
    cout << "} " << (int)'}' << ':' << (int)L'}' << endl;
    cout << "\" " << (int)'"' << ':' << (int)L'"' << endl;
    cout << "  " << (int)' ' << ':' << (int)L' ' << endl;
    cout << ". " << (int)'.' << ':' << (int)L'.' << endl;
    cout << ", " << (int)',' << ':' << (int)L',' << endl;
    cout << "_ " << (int)'_' << ':' << (int)L'_' << endl;
    cout << ": " << (int)':' << ':' << (int)L':' << endl;
    cout << "; " << (int)';' << ':' << (int)L';' << endl;
    cout << "\\n"<< (int)'\n'<< ':' << (int)L'\n'<< endl;
    cout << "-"<< (int)'-'<< ':' << (int)L'-'<< endl;
    cout << "+"<< (int)'+'<< ':' << (int)L'+'<< endl;

    return 0;
}
