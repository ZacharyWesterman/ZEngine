//#include "engine/game.h"

//#include "core/sorted_array.h"

#include "core/stringUtils/char_is_alphanumeric.h"
#include "core/stringUtils/char_is_whiteSpace.h"

#include "script/basic_expression.h"

#include "core/loadLibrary.h"

#include <windows.h>

#include <iostream>
using namespace std;

#include "script/operators/oper_t.h"
#include "core/array.h"

#include "file/list_files_in_dir.h"

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
    core::array< script::oper::oper_t<char>* > operators;

    if (load_opers(operators, "operators/std_operators.dll"))
    {
        //script::basic_expression<char> expr (operators);


        cout << "loaded successfully" << endl;
    }


    core::string<char> expr = "cat*-dog + 4 * -2.5";

    bool is_number = false;

    core::array< core::string<char> > fragment;


    core::string<char> thisString;

    for (int i=0; i<expr.length(); i++)
    {
        /*bool found_num = core::is_alphanumeric(expr[i]) || (expr[i] == (char)46); //decimal point

        if (!is_number && found_num)
        {
            is_number = true;

            fragment.add(thisString);

            thisString.clear();
        }
        else if (is_number && !found_num)
        {
            is_number = false;

            fragment.add(thisString);

            thisString.clear();
        }


        thisString += expr[i];*/
    }

    if (thisString.length() > 0)
        fragment.add(thisString);



    for (int i=0; i<fragment.size(); i++)
        cout << fragment[i].str() << endl;



    cout << "\nDone.\n";

    cout << endl;
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
