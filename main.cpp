//#define CPL_CHAR wchar_t

//#define Z_USE_DOUBLE
#include <iostream>

#include <z/core.h>
#include <z/math.h>
#include <z/file/reader.h>

#include <z/script/compiler/lang.ZScript/lang.keywords.h>
#include <z/script/compiler/lang.ZScript/lang.operators.h>
#include <z/script/compiler/lang.ZScript/lang.comments.h>
#include <z/script/compiler/lang.ZScript/lang.syntax.h>

#include <z/script/compiler.h>




//using namespace std;


using namespace z;
using namespace script;

using std::cout;
using std::endl;

///debug

void printErrors(const core::array< script::error >& error_buffer)
{
    for (int e=0; e<error_buffer.size(); e++)
    {
        cout << "Error ";
        cout << error_buffer[e].fullMessage().str();
        cout << endl;
    }
}

void printIdents(const core::array< compiler::ident_t >& idents)
{
    for (int i=0; i<idents.size(); i++)
    {
        cout << compiler::symTypeStr[idents[i].type];
        if (idents[i].meta)
            cout << " Mt=" << ((core::string<CPL_CHAR>*)idents[i].meta)->narrow().str();
        if (idents[i].value.type())
            cout << " Va=" << idents[i].value.string().narrow().str();
        cout << endl;
    }
}


int main(int argc, char* argv[])
{
    core::array<compiler::keyword>* keywords = genKeywords();
    core::array<compiler::oper>* operators = genOperators();
    core::array< core::string<char> >* comments = genCommentRules();

    core::sortedRefArray< core::string<CPL_CHAR>* > symbol_table;
    core::sortedRefArray< core::string<char>* > file_list;

    core::array< z::script::compiler::ident_t > idents;

    core::string<char> file = "test.txt";

    z::file::reader<CPL_CHAR> Reader;
    Reader.set(file);
    Reader.read();
    core::string<CPL_CHAR> input (Reader.getContents());
    Reader.clear();

    //cout << input.narrow().str() << "\n\n";


    z::script::compiler::scanner Scanner(&symbol_table,
                                               keywords,
                                               operators,
                                               comments,
                                               &file,
                                               &idents);

    Scanner.linkInput(&input);

    Scanner.scan();

    printErrors(Scanner.error_buffer);

    //printIdents(idents);


    core::array< compiler::syntaxRule* >* syntax;
    syntax = genSyntaxRules();

    compiler::syntaxRule* program;
    program = genProgramRule();

    z::script::compiler::lexer Lexer(syntax, program);
    Lexer.linkInput(&idents);

    //cout << program << endl;

    Lexer.lex();

    //compiler::phrase_t* AST = Lexer.moveResultAST();
    printErrors(Lexer.error_buffer);



    delete operators;
    delete keywords;
    delete[] comments;
    for (int i=0; i<syntax->size(); i++)
        delete syntax->at(i);
    delete syntax;
    delete program;

    //compiler::deleteNode(AST);

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
