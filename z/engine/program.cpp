#include "program.h"

#include "driver.h"
#include "instruction.h"

#include <z/core/array.h>
#include <z/core/stream.h>

namespace z
{
	namespace engine
	{
		void program::reset(bool autoRun)
		{
			nextInstruction = 0;
			programIPS = 0;
			isRunning = autoRun;
			reportErrorLevel = NO_ERROR;
		}

		program::~program()
		{
			this->dump();
		}

		program::program(const program* thisParent, core::array<instruction*>* init) : parent(thisParent), instructions(init)
		{
			this->reset((bool)init);
		}

		void program::dump()
		{
			if (instructions)
			{
				for (Int i=0; i<(instructions->length()); i++)
					delete instructions->at(i);
				delete instructions;

				instructions = NULL;
			}
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

		bool program::read(core::inputStream<byte>* input)
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

				if (err <= reportErrorLevel)
				{
					//report the error
				}

				nextInstruction = instr->next(this);
			}
		}
	}
}
