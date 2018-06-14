#pragma once
#ifndef INSTR_MEMALLOC_H_INCLUDED
#define INSTR_MEMALLOC_H_INCLUDED

#include "instruction.h"
#include "inst.h"

namespace z
{
	namespace engine
	{
		class memalloc : public instruction
		{
		private:
			memID amount;

		public:
			~memalloc();

			memalloc(memID);

			void serialIn(core::inputStream<byte>*);

			void execute(program*, driver*);

			void serialOut(core::outputStream<byte>*);
		};
	}
}

#endif //INSTR_MEMALLOC_H_INCLUDED
