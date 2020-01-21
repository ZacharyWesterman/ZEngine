#include <z/core.hpp>
#include <z/file.hpp>
#include <z/util.hpp>
#include <z/system/console.hpp>

#include "z/compiler/scanRule.hpp"
#include "z/compiler/scanner.hpp"
#include "z/compiler/loggable.hpp"


int main(int argc, char** argv)
{
	z::system::console console;
	if (argc < 2)
	{
		zstring("No input file specified!").writeln(console);
		return 1;
	}

	z::compiler::scanner scanner(console);

	scanner.addRule({"\\.data\\b", 1, [] (zstring& text, z::compiler::scanner& scanner)
		{
			scanner.note("Begin data section");
			scanner.logLine(text.length());
		}
	});

	scanner.addRule({"\\.txt\\b", 2, [] (zstring& text, z::compiler::scanner& scanner)
		{
			scanner.note("Begin program section");
			scanner.logLine(text.length());
		}
	});

	scanner.addRule({"[+-]?\\d+[+-]\\d+i|[+-]?\\d+i", 3});//complex nums
	scanner.addRule({"\\d+\\.\\d*", 4});//floats
	scanner.addRule({"\\d+", 5});//integers

	scanner.addRule({"\\b(complex|array|int|float|string|c|a|i|f|s):", 6});

	scanner.addRule({"\\b(load|nop|add)\\b", 7});

	scanner.addRule({"\\$\\d+", 8});//memory access

	scanner.addRule({"\\s+", -1});//white space is not an error!


	scanner.file = argv[1];
	z::file::inputStream input (scanner.file);
	scanner.scan(input);

	scanner.log(zstring("Scanned with ") + scanner.errors() + " errors, " + scanner.warnings() + " warnings.");

	return 0;
}
