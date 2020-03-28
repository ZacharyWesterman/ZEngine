#pragma once
#include <z/util/regex.hpp>

namespace z
{
	namespace compiler
	{
		class scanner;

		class scanRule
		{
		private:
			z::util::regex matchPattern;
			int matchID;
			void (*onMatch)(zstring&, scanner&);

		public:
			scanRule(){};
			scanRule(const zstring& pattern, int resultID, void (*resultOnMatch)(zstring&, scanner&) = NULL);

			friend scanner;
		};
	}
}
