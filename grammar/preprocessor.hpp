#pragma once
#include <z/core/string.hpp>
#include <z/core/stream.hpp>

namespace grammar
{
	bool preprocess(const zstring& filename, z::core::outputStream& output) noexcept;
}
