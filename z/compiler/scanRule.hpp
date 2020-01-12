#pragma once
#include <z/util/regex.h>

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
			bool matchTypeSuccess;

		public:
			scanRule() {};

			scanRule(const zstring& pattern, int resultID, void (*resultOnMatch)(zstring&, scanner&) = NULL, bool resultIsSuccess = true);

			scanRule(const scanRule&) = delete;
			scanRule& operator=(const scanRule&) = delete;

			scanRule(scanRule&&) = default;
			scanRule& operator=(scanRule&& other) = default;

			friend scanner;
		};
	}
}
