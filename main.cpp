#include "z/core/string.h"

#include "z/script/scanner.h"

#include "z/script/load_operators.h"

#include <iostream>
using namespace std;

#include "z/core/sorted_ref_array.h"

int main()
{
    z::core::string<char> s1("string1"), s2("string2"), full;

    full = s1 << s2;

    for (int i=0; i<full.length(); i++)
        cout << (int)full[i] << endl;

    //load operators
    /*z::core::sorted_ref_array< z::script::oper::oper_t<char>* > operators;

    z::script::load_operators(&operators, "operators");

    //operators.sort();

    //for (int i=0; i<operators.size(); i++)
      //  cout << operators[i]->str().str() << endl;

    //test the pre-parser
    z::script::scanner<char>* S;
    S = new z::script::scanner<char>(operators);



    z::core::string<char> input = "\"ten \\nis \" + 10";

    S->scan(input);
    //S->clean();

    cout << input.str() << "\n\n";

    for (int i=0; i<S->arr.size(); i++)
    {
        cout << S->arr[i].name.str() << "\t(" << S->arr[i].type;
        cout << ")\t[" << S->arr[i].line << ',' << S->arr[i].column << "]\n";
    }


    delete S;




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
    cout << "+"<< (int)'+'<< ':' << (int)L'+'<< endl;*/

    return 0;
}
