#include <z/core.h>
#include <z/math.h>

#include "z/script/parser/includeIterator.h"

#include "z/script/parser/constantFolder.h"

#include "z/script/function_t.h"

#include <iostream>
//using namespace std;

using namespace z;
using namespace script;

using std::cout;
using std::endl;


class func_sin : public function_t<char>
{
public:
    func_sin() : function_t<char>("sin", 1, 1, true) {}
    ~func_sin() {}


    error_flag addParam(const data_t<char>& next_param)
    {
        if (next_param.type() != data::VALUE)
            return error::PARAM_MUST_VALUE;

        params.add(next_param);

        return error::NONE;
    }


    bool call(const core::timeout& time)
    {
        if (params.size())
            return_value = sin(params[0].complex());

        return true;
    }
};

class func_log : public function_t<char>
{
public:
    func_log() : function_t<char>("log", 1, 2, true) {}
    ~func_log() {}


    error_flag addParam(const data_t<char>& next_param)
    {
        if (next_param.type() != data::VALUE)
            return error::PARAM_MUST_VALUE;
        else if ((next_param.real() < 0) || (next_param.imag() < 0))
            return error::PARAM_MUST_POSITIVE;

        params.add(next_param);

        return error::NONE;
    }


    bool call(const core::timeout& time)
    {
        if (params.size() == 1)
            return_value = log10(params[0].complex());
        else if (params.size() == 2)
            return_value = log(params[0].complex()) /
                           log(params[1].complex());

        return true;
    }
};


int main(int argc, char* argv[])
{
    //core::sortedArray

    function_t<char>* f;

    f = new func_log;

    for (double x = 1; x < 30; x *= math::e)
    {
        f->addParam(x);
        f->addParam(math::e);
        f->call(-1);

        cout << (f->result().string().str()) << " : " << x << endl;

        f->clear();
    }

    delete f;

    /*char c_in[128];

    cout << "Input:\n\n";

    cin.getline(c_in, 128, '\n');


    core::sortedRefArray< core::string<char>* > symbol_table;

    //test the include iterator
    z::script::includeIterator<char> sscan(&symbol_table);


    z::core::string<char> input = c_in;
    //cout << input.str() << endl;

    sscan.setInput(input, false);
    //sscan.setOutput(identifiers);

    z::core::timeout time (-1);

    int iter = 1;
    while (!sscan.build(time))
    {
        iter++;
        time.reset();
    }

    sscan.printErrors();

    cout << "\n------------------------------------\n\n";
    cout << "AST before folding:\n\n";

    z::script::phrase_t<char>* AST = sscan.moveResultAST();

    z::script::print_lex_ast(0, AST);

    cout << "\n------------------------------------\n\n";
    cout << "AST after folding:\n\n";

    z::script::constantFolder<char> cFolder;

    cFolder.setInput(AST);

    time.reset();
    while (!cFolder.fold(time))
    {
        iter++;
        time.reset();
    }


    z::script::print_lex_ast(0, AST);


    if (AST)
        delete AST;

    std::cout << "\nDone in " << iter << " iterations.\n";*/

    //S->clean();

    //cout << "Scanned in " << iter << " iterations.\n";

    //sscan.error() ? (cout << "Found error(s)" << endl) : (cout << "No errors." << endl);


    //S->clean();

    //cout << "Lexed in " << iter << " iterations.\n";

    //llexr.error() ? (cout << "Found error(s)" << endl) : (cout << "No errors." << endl);*/

    /*
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
    cout << "- "<< (int)'-'<< ':' << (int)L'-'<< endl;
    cout << "+ "<< (int)'+'<< ':' << (int)L'+'<< endl;
    cout << "0 "<< (int)'0'<< ':' << (int)L'0'<< endl;
    cout << "1 "<< (int)'1'<< ':' << (int)L'1'<< endl;
    cout << "7 "<< (int)'7'<< ':' << (int)L'7'<< endl;
    cout << "9 "<< (int)'9'<< ':' << (int)L'9'<< endl;
    cout << "a "<< (int)'a'<< ':' << (int)L'a'<< endl;
    cout << "i "<< (int)'i'<< ':' << (int)L'i'<< endl;
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
