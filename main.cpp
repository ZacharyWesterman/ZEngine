#include <z/system/console.h>
#include "engine/heap.hpp"

using z::util::generic;

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	z::system::console console;

	engine::heap memory;

	size_t memloc = 2;
	bool result = memory.set(memloc << 1, new z::util::genericComplex(std::complex<double>(123.45,2)));

	if (result)
	{
		auto args = memory.returnVal();
		(zpath("args = \"")+(args->complex())+"\"").writeln(console);
	}
	else
	{
		(zpath("Unable to set memory location ")+memloc).writeln(console);
	}


	return 0;
}
