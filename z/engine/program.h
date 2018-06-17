#pragma once
#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include <z/z.h>
#include <stack>

#include "errorLevel.h"

#ifndef NULL
#define NULL 0
#endif

namespace z
{
	namespace core
	{
		template <typename>
		class array;

		template <typename>
		class inputStream;

		template <typename>
		class string;
	}

	namespace engine
	{
		class instruction;
		class driver;

		class program
		{
		private:
			// const core::string<Char> file;

			const program* parent;
			core::array<instruction*>* instructions;

			bool isRunning;
			Float programIPS;

			Int nextInstruction;
			std::stack<Int> returnIndex;

			errorLevel reportErrorLevel;

			void reset(bool);

		public:
			~program();

			program(const program* thisParent = NULL, core::array<instruction*>* init = NULL);

			void dump();

			bool operator==(const program& other) const;

			Int next() const;

			void exit();
			bool running() const;

			bool read(core::inputStream<byte>*);

			void run(const driver*);
		};
	}
}

#endif //PROGRAM_H_INCLUDED
