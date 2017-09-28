//#define Z_USE_DOUBLE
#include <iostream>

#include <z/core.h>
#include <z/math.h>
#include <z/file/reader.h>

#include <z/script/compiler/scanner.h>


//using namespace std;


using namespace z;
using namespace script;

using std::cout;
using std::endl;

///debug

void printErrors(const core::array< error >& error_buffer)
{
    for (int e=0; e<error_buffer.size(); e++)
    {
        cout << "Error ";
        cout << error_buffer[e].fullMessage().str();
        cout << endl;
    }
}



int main(int argc, char* argv[])
{
    /*core::array< script::command<char>* > commands;
    core::array< script::function<char>* > functions;

    commands.add(new cmd_print);
    commands.add(new cmd_print_error);

    functions.add(new func_sin);
    functions.add(new func_log);*/

    //char c_in[128];

    //cout << "Input:\n\n";

    //cin.getline(c_in, 128, '\n');

    core::sortedRefArray< core::string<char>* > symbol_table;
    core::sortedRefArray< core::string<char>* > file_list;

    core::string<char> file = "test.txt";

    z::file::reader<char> Reader;
    Reader.set(file);
    Reader.read();
    core::string<char> input(Reader.getContents());
    Reader.clear();


    z::script::compiler::scanner<char> Scanner(&symbol_table);
    Scanner.file = &file;
    Scanner.setInput(input);

    core::array< z::script::compiler::ident_t<char> > idents;

    Scanner.setOutput(idents);


    Scanner.scan();

    printErrors(Scanner.error_buffer);

    //test the include iterator
    /*z::script::compiler::includeIterator<char> genAST(&symbol_table, &file_list);

    //cout << input.str() << endl;

    z::core::string<char> input = "test.txt";

    genAST.setInput(input, true);
    //sscan.setOutput(identifiers);

    z::core::timeout time (-1);

    int iter = 1;
    while (!genAST.build(time))
    {
        iter++;
        time.reset();
    }

    printErrors(genAST.error_buffer);

    //cout << "\n------------------------------------\n\n";
    //cout << "AST before folding:\n\n";



    z::script::compiler::phrase_t<char>* AST = genAST.moveResultAST();

    //z::script::compiler::print_lex_ast(0, AST);

    //cout << "\n------------------------------------\n\n";
    //cout << "AST after folding:\n\n";

    z::script::compiler::constantFolder<char> cFolder;

    cFolder.setInput(AST);

    time.reset();
    while (!cFolder.fold(time))
    {
        iter++;
        time.reset();
    }

    //z::script::compiler::print_lex_ast(0, AST);

    printErrors(cFolder.error_buffer);


    //cout << "\n------------------------------------\n\n";
    cout << "AST after semantic analysis:\n\n";

    z::script::compiler::semanticAnalyzer<char> semantics(commands, functions);

    semantics.setInput(AST);

    time.reset();
    while (!semantics.analyze(time))
    {
        iter++;
        time.reset();
    }


    z::script::compiler::print_lex_ast(0, AST);

    printErrors(semantics.error_buffer);


    if (AST)
        script::compiler::delete_ast(AST);*/

    //std::cout << "\nDone in " << iter << " iterations.\n";



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


/*class func_sin : public function<char>
{
public:
    func_sin() : function<char>("sin", true, 1, 1) {}
    ~func_sin() {}


    errorFlag addParam(const generic<char>& next_param)
    {
        if (next_param.type() != data::VALUE)
            return error::PARAM_MUST_VALUE;

        params.add(next_param);

        return error();
    }


    bool call(const core::timeout& time)
    {
        if (params.size())
            return_value = sin(params[0].complex());

        return true;
    }
};

class func_log : public function<char>
{
public:
    func_log() : function<char>("log", true, 0, 0) {}
    ~func_log() {}


    errorFlag addParam(const generic<char>& next_param)
    {
        if (next_param.type() != data::VALUE)
            return error::PARAM_MUST_VALUE;
        else if ((next_param.real() < 0) || (next_param.imag() < 0))
            return error::PARAM_MUST_POSITIVE;

        params.add(next_param);

        return error();
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



class cmd_print : public command<char>
{
    public:
    cmd_print() : command<char>({"print"}) {}
    ~cmd_print() {}


    bool call(const core::timeout& time)
    {
        for (int i=0; i<params.size(); i++)
            cout << params[i].string().str() << " ";
        cout << endl;

        return true;
    }
};

class cmd_print_error : public command<char>
{
    public:
    cmd_print_error() : command<char>({"print","error"}) {}
    ~cmd_print_error() {}


    bool call(const core::timeout& time)
    {
        cerr << "ERR: ";
        for (int i=0; i<params.size(); i++)
            cerr << params[i].string().str() << " ";
        cerr << endl;

        return true;
    }
};*/
