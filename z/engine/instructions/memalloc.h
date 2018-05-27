#pragma once
#ifndef INSTR_MEMALLOC_H_INCLUDED
#define INSTR_MEMALLOC_H_INCLUDED

#include "instruction.h"

namespace z
{
	namespace engine
	{
		class memalloc : public instruction
		{
		private:

		public:
			memalloc();
			~memalloc();

			void execute(program*, driver*);
		};
	}
}

#endif //INSTR_MEMALLOC_H_INCLUDED
