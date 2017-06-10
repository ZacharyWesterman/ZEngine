#include <z/core.h>
#include <z/math.h>

#include "z/script/parser/includeIterator.h"

#include "z/script/variables/data_t.h"

#include <iostream>
//using namespace std;

using namespace z;


int main(int argc, char* argv[])
{
    script::data_t<char> data;

    data.d_value = std::complex<double>(0.9999999996, 1);

    cout << core::string<char>(data.d_value).str() << endl;

    data.d_string = core::string<char>("Hello");
    cout << data.d_string.str() << endl;

    /*char c_in[128];

    cout << "Input:\n\n";

    cin.getline(c_in, 128, '\n');

    core::sorted_array< core::string<char> > commands;
    commands.add("print");
    commands.add("exit");
    commands.add("delay");

    core::sorted_array< core::string<char> > functions;
    functions.add("log");
    functions.add("ln");
    functions.add("sum");


    core::sorted_ref_array< core::string<char>* > symbol_table;

    //z::script::scanner<char> S(&symbol_table, &operators, &commands, &functions);

    //core::array< z::script::ident_t<char> > identifiers;

    //test the include iterator
    z::script::includeIterator<char> sscan(&symbol_table, &commands, &functions);


    z::core::string<char> input = c_in;
    //cout << input.str() << endl;

    ///debug
    cout << "\n------------------------------------\n\n";
    cout << "Syntax Tree:\n\n";

    sscan.setInput(input, false);
    //sscan.setOutput(identifiers);

    z::core::timeout time (-1);

    int iter = 1;
    while (!sscan.build(time))
    {
        iter++;
        time.reset();
    }

    sscan.printErrors();*.

    //std::cout << "\nDone.\n";

    //S->clean();

    //cout << "Scanned in " << iter << " iterations.\n";

    //sscan.error() ? (cout << "Found error(s)" << endl) : (cout << "No errors." << endl);


    //S->clean();

    //cout << "Lexed in " << iter << " iterations.\n";

    //llexr.error() ? (cout << "Found error(s)" << endl) : (cout << "No errors." << endl);*/


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
    cout << "\\ "<< (int)'\\'<< ':' << (int)L'\\'<< endl;
    cout << "* "<< (int)'*'<< ':' << (int)L'*'<< endl;*/

    return 0;
}
