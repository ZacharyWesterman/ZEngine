#pragma once
#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED

#include <z/z.h>
#include "errorLevel.h"
#include "inst.h"

namespace z
{
	namespace core
	{
		template <typename>
		class string;

		template <typename>
		class inputStream;

		template <typename>
		class outputStream;
	}

	namespace engine
	{
		class driver;
		class program;

		class instruction
		{
		private:

		public:
			virtual ~instruction() {};


			virtual errorLevel execute(const program*, const driver*) = 0;
			virtual Int next(const program*);

			virtual core::string<Char> errorMessage();

			//Must be able to read instruction from a stream, not including opcode.
			virtual void serialIn(core::inputStream<byte>*) = 0;

			//write instruction to a stream, must include opcode.
			virtual void serialOut(core::outputStream<byte>*) = 0;
		};
	}
}

#endif //INSTRUCTION_H_INCLUDED
