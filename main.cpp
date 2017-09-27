//#define Z_USE_DOUBLE
#include <iostream>

#include <z/core.h>
#include <z/math.h>

#include <z/script.h>


//using namespace std;


using namespace z;
using namespace script;

using std::cout;
using std::endl;

///debug
        template <typename CHAR>
        void printErrors(const core::array< error >& error_buffer)
        {
            for (int e=0; e<error_buffer.size(); e++)
            {
                error perr = error_buffer[e];

                cout << "Error ";

                if (error_buffer[e].file)
                {
                    cout << "in \""
                         << error_buffer[e].file->str()
                         << "\" ";
                }

                    cout << "at " << perr.line+1
                         << "," << perr.column+1
                         << " : ";


                switch (perr.err)
                {
                case error::NUMBER_ILLEGAL_CHAR:
                    cout << "'" << perr.extra_data.str()
                         << "' contains illegal characters.";
                    break;

                case error::UNKNOWN_OPERATOR:
                    cout << "Unknown operator \"" << perr.extra_data.str() <<
                            "\".";
                    break;

                case error::AMBIGUOUS_EXPR:
                    cout << "The expression \"" << perr.extra_data.str() <<
                            "\" contains illegal characters.";
                    break;

                case error::SYNTAX_ERROR:
                    cout << "Syntax error.";
                    break;

                case error::INVALID_INCLUDE:
                    cout << "Include statement with no file name.";
                    break;

                case error::INCLUDE_LOAD_FAILED:
                    cout << "Unable to load include file.";
                    break;

                case error::UNEXPECTED_OPERATOR:
                    cout << "Unexpected operator.";
                    break;

                case error::STMT_OUTSIDE_FUNCTION:
                    cout << "Statement outside of function declaration.";
                    break;

                case error("Too many parameters"):
                    cout << "Statement contains too many parameters.";
                    break;

                case error("Too few parameters"):
                    cout << "Statement contains too few parameters.";
                    break;

                case error::EXPECTED_PARAMETER:
                    cout << "Expected parameters for statement.";
                    break;

                case error::VARIABLE_REDECLARED:
                    cout << "Variable \"" << perr.extra_data.str() << "\" declared previously.";
                    break;

                case error::VARIABLE_UNDECLARED:
                    cout << "Variable \"" << perr.extra_data.str() << "\" undeclared.";
                    break;

                case error::UNEXPECTED_SEMICOLON:
                    cout << "Unexpected semicolon.";
                    break;

                case error::TYPE_MISMATCH:
                    cout << "Expression contains mismatched types.";
                    break;

                case error::FUNCTION_REDECLARED:
                    cout << "Function \"" << perr.extra_data.str() << "\" declared previously.";
                    break;

                case error::FUNCTION_ALREADY_TYPE:
                    cout << "A type with the name \"" << perr.extra_data.str()
                         << "\" already exists.";
                    break;

                case error::TYPE_ALREADY_FUNCTION:
                    cout << "A function with the name \"" << perr.extra_data.str()
                         << "\" already exists.";
                    break;

                case error::TYPE_UNDEFINED:
                    cout << "Unknown type \"" << perr.extra_data.str() << "\".";
                    break;

                default:
                    cout << "Unhandled error " << (int)perr.err << ".";
                }

                cout << endl;

            }
        }

class func_sin : public function<char>
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
};

int main(int argc, char* argv[])
{
    core::array< script::command<char>* > commands;
    core::array< script::function<char>* > functions;

    commands.add(new cmd_print);
    commands.add(new cmd_print_error);

    functions.add(new func_sin);
    functions.add(new func_log);

    //char c_in[128];

    //cout << "Input:\n\n";

    //cin.getline(c_in, 128, '\n');

    core::sortedRefArray< core::string<char>* > symbol_table;
    core::sortedRefArray< core::string<char>* > file_list;

    //test the include iterator
    z::script::compiler::includeIterator<char> genAST(&symbol_table, &file_list);

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
        script::compiler::delete_ast(AST);

    std::cout << "\nDone in " << iter << " iterations.\n";



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
