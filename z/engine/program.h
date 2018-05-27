#pragma once
#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include <z/z.h>

namespace z
{
	namespace core
	{
		template <typename T>
		class array;
	}

	namespace engine
	{
		class instruction;

		class program
		{
		private:
			core::array<instruction*>* instructions;

		public:
			program();
			~program();

			bool operator==(const program& other) const;
		};
	}
}

#endif //PROGRAM_H_INCLUDED
