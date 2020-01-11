#pragma once
#include <z/core/array.h>
#include <z/core/stream.h>

#include "scanRule.hpp"

namespace z
{
	namespace compiler
	{
		class scanner
		{
		private:
			size_t line;
			size_t column;

		public:
			z::core::array<scanRule> rules;

			bool scanOnce(z::core::inputStream& stream);

			void scan(z::core::inputStream& stream);

			void error(const zstring& message);
		};
	}
}
