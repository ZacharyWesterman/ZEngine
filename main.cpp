#include "z/core/string.h"

//#include "z/script/preParser.h"

#include "z/script/load_operators.h"

#include <iostream>
using namespace std;



int main()
{
    //load operators
    z::core::array< z::script::oper::oper_t<char>* > operators;

    z::script::load_operators(operators, "operators");



    //test the pre-parser
    /*script::preParser<char> preP (operators);

    cout << "err[" << preP.split("print 10{\"arr[10]=\",arr[10]}") << "]\n";

    for (int i=0; i<preP.arr.size(); i++)
    {
        cout << preP.arr[i].str.str() << endl;
    }*/




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
