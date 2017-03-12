#include <z/core.h>
#include <z/math.h>

//#include "z/script/scanner.h"

//#include "z/script/load_operators.h"

#include "z/script/variables/datatype.h"

#include "z/script/parser/scan_iterator.h"

#include <iostream>
using namespace std;

using namespace z;

/*void charstream(const core::stream<char>& input)
{
    for (int i=0; i<input.length(); i++)
        cout << (int)input.str()[i] << endl;
}*/

int main(int argc, char* argv[])
{

    core::sorted_array< core::string<char> > operators;
    operators.add("+");
    operators.add("-");
    operators.add("*");
    operators.add("/");

    core::sorted_array< core::string<char> > commands;
    commands.add("print");
    commands.add("exit");
    commands.add("delay");

    core::sorted_array< core::string<char> > functions;
    functions.add("log");


    z::script::scan_iterator<char> SI(&operators, &commands, &functions);

    //test the scanner
    /*z::script::scanner<char>* S;
    S = new z::script::scanner<char>(&operators, &commands, &functions);



    z::core::string<char> input = "print 0xffffff && 10";

    cout << ((S->scan(input) && S->clean()) ? "No errors" : "Found errors") << endl;


    cout << input.str() << "\n\n";

    for (int i=0; i<S->identifiers.size(); i++)
    {
        cout << S->identifiers[i].name.str() << "\t(" << S->identifiers[i].type;
        cout << ")\t[" << S->identifiers[i].line << ',' << S->identifiers[i].column << "]";
        cout << " {" << S->identifiers[i].err << "}\n";
    }


    delete S;*/




    cout << "\nDone.\n";

    /*cout << endl;
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
    cout << "- "<< (int)'-'<< ':' << (int)L'-'<< endl;
    cout << "+ "<< (int)'+'<< ':' << (int)L'+'<< endl;
    cout << "0 "<< (int)'0'<< ':' << (int)L'0'<< endl;
    cout << "1 "<< (int)'1'<< ':' << (int)L'1'<< endl;
    cout << "7 "<< (int)'7'<< ':' << (int)L'7'<< endl;
    cout << "9 "<< (int)'9'<< ':' << (int)L'9'<< endl;
    cout << "a "<< (int)'a'<< ':' << (int)L'a'<< endl;
    cout << "f "<< (int)'f'<< ':' << (int)L'f'<< endl;
    cout << "z "<< (int)'z'<< ':' << (int)L'z'<< endl;
    cout << "A "<< (int)'A'<< ':' << (int)L'A'<< endl;
    cout << "F "<< (int)'F'<< ':' << (int)L'F'<< endl;
    cout << "Z "<< (int)'Z'<< ':' << (int)L'Z'<< endl;
    cout << "= "<< (int)'='<< ':' << (int)L'='<< endl;
    cout << "\\ "<< (int)'\\'<< ':' << (int)L'\\'<< endl;*/

    return 0;
}
