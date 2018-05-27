#pragma once
#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED

namespace z
{
	namespace engine
	{
		class program;

		class instruction
		{
		private:

		public:
			virtual ~instruction() = 0;

			virtual void execute(program*, driver*) = 0;
		};
	}
}

#endif //INSTRUCTION_H_INCLUDED
