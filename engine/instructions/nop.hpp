#pragma once

#include "../instruction.hpp"
#include "../ops.hpp"

namespace engine
{
	class nop : public instruction
	{
	public:
		nop() : instruction(NOP) {}

		void serialIn(z::core::inputStream& stream) {(void)stream;}
		void serialOut(z::core::outputStream& stream) {(void)stream;}
	};
}
