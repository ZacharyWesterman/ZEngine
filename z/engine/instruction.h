#pragma once
#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED

#include "errorLevel.h"
#include <z/z.h>

namespace z
{
	namespace core
	{
		template <typename T>
		class string;
	}

	namespace engine
	{
		class driver;
		class program;

		class instruction
		{
		private:

		public:
			virtual ~instruction() = 0;

			virtual errorLevel execute(const program*, const driver*) = 0;
			virtual Int next(const program*);

			virtual core::string<Char> errorMessage();
		};
	}
}

#endif //INSTRUCTION_H_INCLUDED
