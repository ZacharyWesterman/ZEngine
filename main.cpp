#include "z/engine/program.h"

#include <z/core/array.h>

#include "z/engine/instructions/memalloc.h"

using namespace z;

core::array<engine::instruction*>* genInstructions();

int main(int argc, char** argv)
{
	// z::engine::driver Driver(argc, argv);

	core::array<engine::instruction*>* instructions = genInstructions();

	z::engine::program tmp(NULL, instructions);

	return 0;
}

core::array<engine::instruction*>* genInstructions()
{
	core::array<engine::instruction*>* instructions;

	instructions = new core::array<engine::instruction*>;

	instructions->add(new engine::memalloc(0));

	return instructions;
}
