#include <z/core.hpp>
#include <z/file.hpp>
#include <z/util.hpp>
#include <z/system.hpp>

#include "z/compiler/scanRule.hpp"
#include "z/compiler/scanner.hpp"

enum
{
	EXPR_VARIABLE,
	EXPR_NUMBER,
	EXPR_ADD,
	EXPR_SUBTRACT,
	EXPR_MULTIPLY,
	EXPR_DIVIDE,
	EXPR_MODULUS,
	EXPR_INTDIVIDE,
	EXPR_POWER,

	EXPR_EQ,
	EXPR_NE,
	EXPR_GE,
	EXPR_GT,
	EXPR_LE,
	EXPR_LT,
	EXPR_NOT,
	EXPR_AND,
	EXPR_OR,

	EXPR_LPAREN,
	EXPR_RPAREN,

	EXPR_TERNARY,
	EXPR_FALLTHROUGH,
};

void print(const z::core::array<z::compiler::identifier>& idents)
{
	z::system::stdout stdout;

	size_t max = 0;
	for (auto& ident : idents)
	{
		if (ident.text->length() > max) max = ident.text->length();
	}

	for (auto& ident : idents)
	{
		(zstring(*(ident.text)).padRight(" ",max) + " (" + ident.line + ',' + ident.column + ')').writeln(stdout);
	}
}

int main(int argc, char** argv)
{
	z::system::stderr stderr;
	z::system::stdout stdout;
	if (argc < 2)
	{
		zstring("No input file specified!").writeln(stderr);
		return 1;
	}

	z::compiler::scanner scanner(stderr);

	scanner.addRule({"[\\a_]\\w*", EXPR_VARIABLE});
	scanner.addRule({"\\d+(\\.\\d*)?|\\.\\d*", EXPR_NUMBER});
	scanner.addRule({"\\+", EXPR_ADD});
	scanner.addRule({"-", EXPR_SUBTRACT});
	scanner.addRule({"\\*", EXPR_MULTIPLY});
	scanner.addRule({"/", EXPR_DIVIDE});
	scanner.addRule({"%", EXPR_MODULUS});
	scanner.addRule({"\\", EXPR_INTDIVIDE});
	scanner.addRule({"^", EXPR_POWER});
	scanner.addRule({"=", EXPR_EQ});
	scanner.addRule({"!=", EXPR_NE});
	scanner.addRule({"<=", EXPR_GE});
	scanner.addRule({"<", EXPR_GT});
	scanner.addRule({"<=", EXPR_LE});
	scanner.addRule({"<", EXPR_LT});
	scanner.addRule({"!", EXPR_NOT});
	scanner.addRule({"&", EXPR_AND});
	scanner.addRule({"\\|", EXPR_OR});
	scanner.addRule({"\\(", EXPR_LPAREN});
	scanner.addRule({"\\)", EXPR_RPAREN});
	scanner.addRule({"\\?", EXPR_TERNARY});
	scanner.addRule({":", EXPR_FALLTHROUGH});

	scanner.file = argv[1];
	z::file::inputStream input (scanner.file);

	z::core::sortedRefArray<zstring*> symbolTable;
	auto idents = scanner.scan(input, symbolTable);

	scanner.log(zstring("Scanned with ") + scanner.errors() + " errors, " + scanner.warnings() + " warnings.");

	print(idents);
	// (zstring("valid: {")+z::core::joinDeref(symbolTable,zstring(','))+'}').writeln(stdout);

	return 0;
}
