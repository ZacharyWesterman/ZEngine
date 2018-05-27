#include "program.h"
#include "instruction.h"

#include <z/core/array.h>

namespace z
{
	namespace engine
	{
		program::program()
		{
			instructions = new core::array<instruction*>;
		}

		program::~program()
		{
			delete instructions;
		}

		bool program::operator==(const program& other) const
		{
			return false;
		}
	}
}
