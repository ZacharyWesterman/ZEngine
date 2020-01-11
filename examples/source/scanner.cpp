#include <z/core.h>
#include <z/file.h>
#include <z/util.h>
#include <z/system/console.h>

#include "z/compiler/scanRule.hpp"
#include "z/compiler/scanner.hpp"

z::system::console console;

int main()
{
	z::compiler::scanner scanner;

	scanner.rules.add({"\\.data\\b", 1, [] (zstring& text)
		{
			(void)text;
			zstring("Begin data section").writeln(console);
		}
	});

	scanner.rules.add({"\\.txt\\b", 2, [] (zstring& text)
		{
			(void)text;
			zstring("Begin program section").writeln(console);
		}
	});

	scanner.rules.add({"[+-]?\\d+[+-]\\d+i|[+-]?\\d+i", 3});//complex nums
	scanner.rules.add({"\\d+\\.\\d*", 4});//floats
	scanner.rules.add({"\\d+", 5});//integers

	scanner.rules.add({"\\b(complex|array|int|float|string|c|a|i|f|s):", 6});

	scanner.rules.add({"\\b(load|nop|add)\\b", 7});

	scanner.rules.add({"\\$\\d+", 8});//memory access

	scanner.rules.add({"\\s+", -1});//white space is not an error!


	z::file::inputStream input ("examples/data/program1.txt");
	scanner.scan(input);

	return 0;
}
