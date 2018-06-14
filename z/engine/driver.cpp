#include "driver.h"
#include "program.h"

#include <z/core/array.h>
#include <z/core/string.h>

namespace z
{
	namespace engine
	{
		driver::driver(int argc, char** argv)
		{
			programs = new core::array<program>;
		}

		driver::~driver()
		{
			delete programs;
		}
	}
}
