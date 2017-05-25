#include <z/core.h>
#include <z/math.h>

#include "z/script/parser/includeIterator.h"
//#include "z/script/parser/lexer.h"

//#include "z/script/load_operators.h"

//#include "z/script/variables/datatype.h"

//#include "z/script/parser/scan_iterator.h"

#include <iostream>
//using namespace std;

using namespace z;

/*void charstream(const core::stream<char>& input)
{
    for (int i=0; i<input.length(); i++)
        cout << (int)input.str()[i] << endl;
}*/

int main(int argc, char* argv[])
{

    /*core::sorted_array< core::string<char> > operators;
    operators.add("+");
    operators.add("-");
    operators.add("*");
    operators.add("/");
    operators.add("and");*/

    core::sorted_array< core::string<char> > commands;
    commands.add("print");
    commands.add("exit");
    commands.add("delay");

    core::sorted_array< core::string<char> > functions;
    functions.add("log");


    core::sorted_ref_array< core::string<char>* > symbol_table;

    //z::script::scanner<char> S(&symbol_table, &operators, &commands, &functions);

    //core::array< z::script::ident_t<char> > identifiers;

    //test the include iterator
    z::script::includeIterator<char> sscan(&symbol_table, &commands, &functions);


    z::core::string<char> input = "10 =+- 12 - 42";
    sscan.setInput(input, false);
    //sscan.setOutput(identifiers);

    z::core::timeout time (-1);

    std::cout << "HELLO\n";

    int iter = 1;
    while (!sscan.build(time))
    {
        iter++;
        time.reset();
    }

    std::cout << "\nDone.\n";

    //S->clean();

    //cout << "Scanned in " << iter << " iterations.\n";

    //sscan.error() ? (cout << "Found error(s)" << endl) : (cout << "No errors." << endl);


    /*z::script::lexer<char> llexr;

    iter = 1;
    while (!llexr.lex(time))
    {
        iter++;
        time.reset();
    }*/

    //S->clean();

    //cout << "Lexed in " << iter << " iterations.\n";

    //llexr.error() ? (cout << "Found error(s)" << endl) : (cout << "No errors." << endl);


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
