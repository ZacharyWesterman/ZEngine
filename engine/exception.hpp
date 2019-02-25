#pragma once

namespace engine
{
	class exception
	{
	public:
		const uint8_t type;
		const uint8_t sub;
		const size_t desc;

		exception(uint8_t exceptType, uint8_t exceptSub = 0, size_t memoryDesc = 0) : type(exceptType), sub(exceptSub), desc(memoryDesc) {}
	};
}
