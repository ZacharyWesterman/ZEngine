#include <z/core.h>
#include <z/file.h>
#include <z/util.h>
#include <z/system/console.h>

#include "z/compiler/scanRule.hpp"
#include "z/compiler/scanner.hpp"

int main()
{
	z::compiler::scanner scanner;

	scanner.rules.add({"\\w+", 1, [] (zstring& text)
		{
			z::system::console console;
			(zstring("Match is <") + text + ">").writeln(console);
		}
	});

	scanner.rules.add({"\\s+", 2});//white space is not an error!


	z::file::inputStream input ("examples/data/program1.txt");
	scanner.scan(input);

	return 0;
}
