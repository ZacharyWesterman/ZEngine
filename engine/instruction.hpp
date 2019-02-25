#pragma once

#include <z/core/serializable.h>

namespace engine
{
	class instruction : public z::core::serializable
	{
	public:
		const uint8_t type;

		instruction(uint8_t OPtype) : type(OPtype) {}
		virtual ~instruction() {}
	};
}
