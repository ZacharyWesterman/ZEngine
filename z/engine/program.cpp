#include "program.h"

#include "driver.h"
#include "instruction.h"

#include <z/core/array.h>
#include <z/core/stream.h>

namespace z
{
	namespace engine
	{
		program::program(const program* thisParent): parent(thisParent)
		{
			instructions = new core::array<instruction*>;
			nextInstruction = 0;
			programIPS = 0;

			isRunning = false;
		}

		program::~program()
		{
			delete instructions;
		}

		bool program::operator==(const program& other) const
		{
			return false;
		}

		Int program::next() const
		{
			return nextInstruction;
		}

		void program::exit()
		{
			isRunning = false;
		}

		bool program::running() const
		{
			return isRunning;
		}

		bool program::read(core::inputStream<Char>& input)
		{
			nextInstruction = 0;

			return true;
		}

		void program::run(const driver* thisDriver)
		{
			if (!instructions->isValid(nextInstruction))
				isRunning = false;

			if (isRunning)
			{
				instruction* instr = instructions->at(nextInstruction);

				errorLevel err = instr->execute(this, thisDriver);

				nextInstruction = instr->next(this);
			}
		}
	}
}
