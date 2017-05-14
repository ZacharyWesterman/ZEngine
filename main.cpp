#include <z/core.h>
#include <z/math.h>

#include "z/script/parser/includeIterator.h"

//#include "z/script/load_operators.h"

//#include "z/script/variables/datatype.h"

//#include "z/script/parser/scan_iterator.h"

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
    operators.add("and");

    core::sorted_array< core::string<char> > commands;
    commands.add("print");
    commands.add("exit");
    commands.add("delay");

    core::sorted_array< core::string<char> > functions;
    functions.add("log");


    core::sorted_ref_array< core::string<char>* > symbol_table;

    //z::script::scanner<char> S(&symbol_table, &operators, &commands, &functions);

    core::array< z::script::ident_t<char> > identifiers;

    //test the scanner
    z::script::includeIterator<char>* iiter;
    iiter = new z::script::includeIterator<char>(&symbol_table, &operators, &commands, &functions);


    iiter->setInput("file1.txt", true);

    //z::core::string<char> input = "print  ten 0c1.1 and log 0b1.1";
    //S->setInput(input);
    //S->setOutput(identifiers);

    z::core::timeout time (100);

    int iter = 1;
    while (!iiter->build(time))
    {
        iter++;
        time.reset();
    }

    //S->clean();

    cout << "Scanned in " << iter << " iterations.\n";

    iiter->error() ? (cout << "Found error(s)" << endl) : (cout << "No errors." << endl);
    /*

    //symbol_table.find(&input);

    cout << input.str() << "\n\n";

    for (int i=0; i<identifiers.size(); i++)
    {
        core::string<char>* symbol = (core::string<char>*)(identifiers[i].meta);

        if (symbol)
            cout << symbol->str();
        else if (identifiers[i].type == z::script::ident::NUMERIC_LITERAL)
            cout << "#" << identifiers[i].value;
        else
            cout << "NULL";
        cout << "\t(" << identifiers[i].type;
        cout << ")\t[" << identifiers[i].line;
        cout << ',' << identifiers[i].column << "]";
        cout << " {" << identifiers[i].err << "}\n";
    }

    for (int i=0; i<symbol_table.size(); i++)
        delete symbol_table[i];*/

    delete iiter;




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
    cout << "\\ "<< (int)'\\'<< ':' << (int)L'\\'<< endl;
    cout << "* "<< (int)'*'<< ':' << (int)L'*'<< endl;*/

    return 0;
}
