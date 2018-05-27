#include "instruction.h"

#include "program.h"

#include <z/core/string.h>

namespace z
{
	namespace engine
	{
		Int instruction::next(const program* thisProgram)
		{
			return thisProgram->next() + 1;
		}

		core::string<Char> instruction::errorMessage()
		{
			return core::string<Char>("Unknown error");
		}
	}
}
