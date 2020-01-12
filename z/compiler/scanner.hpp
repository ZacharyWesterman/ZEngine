#pragma once
#include <z/core/array.h>
#include <z/core/stream.h>

#include "scanRule.hpp"
#include "loggable.hpp"

namespace z
{
	namespace compiler
	{
		class scanner : public loggable
		{
		private:
			bool scanOnce(z::core::inputStream& stream);
			z::core::array<scanRule> rules;

		public:
			scanner() : loggable() {};

			void scan(z::core::inputStream& stream);
			void addRule(scanRule&& rule, int scope = 0);
		};
	}
}
