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
			void (*onMatch)(zstring&);
			bool matchTypeSuccess;

		public:
			scanRule() {};

			// template<typename Functor>
			// scanRule(Functor resultOnMatch)
			// {
			//
			// 	onMatch = resultOnMatch;
			// }

			scanRule(const zstring& pattern, int resultID, void (*resultOnMatch)(zstring&) = NULL, bool resultIsSuccess = true);

			scanRule(const scanRule&) = delete;
			scanRule& operator=(const scanRule&) = delete;

			scanRule(scanRule&&) = default;
			scanRule& operator=(scanRule&& other) = default;

			friend scanner;
		};
	}
}
