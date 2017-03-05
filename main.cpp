#include <z/core.h>
#include <z/math.h>

//#include "z/script/scanner.h"

//#include "z/script/load_operators.h"

#include "z/script/variables/datatype.h"

#include "z/script/parser/scanner.h"

#include <iostream>
using namespace std;

using namespace z;

/*void charstream(const core::stream<char>& input)
{
    for (int i=0; i<input.length(); i++)
        cout << (int)input.str()[i] << endl;
}*/

int main()
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


    //test the pre-parser
    z::script::scanner<char>* S;
    S = new z::script::scanner<char>(&operators, &commands, &functions);



    z::core::string<char> input = "print \"me me \\\"big boy\\\"\"";

    S->scan(input);
    S->clean();

    cout << input.str() << "\n\n";

    for (int i=0; i<S->identifiers.size(); i++)
    {
        cout << S->identifiers[i].name.str() << "\t(" << S->identifiers[i].type;
        cout << ")\t[" << S->identifiers[i].line << ',' << S->identifiers[i].column << "]\n";
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
    cout << "+"<< (int)'+'<< ':' << (int)L'+'<< endl;

    return 0;
}
