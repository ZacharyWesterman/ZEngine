#include "memalloc.h"

#include <z/core/stream.h>

namespace z
{
	namespace engine
	{
		memalloc::~memalloc() {}

		memalloc::memalloc(memID memAmount) : amount(memAmount) {}

		void memalloc::execute(program*, driver*)
		{

		}

		void memalloc::serialIn(core::inputStream<byte>* stream)
		{

		}

		void memalloc::serialOut(core::outputStream<byte>* stream)
		{
			
		}
	}
}
