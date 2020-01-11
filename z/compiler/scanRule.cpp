#include "scanRule.hpp"

namespace z
{
	namespace compiler
	{
		scanRule::scanRule(const zstring& pattern, int resultID, void (*resultOnMatch)(zstring&), bool resultIsSuccess) :
			matchPattern(pattern),
			matchID(resultID),
			onMatch(resultOnMatch),
			matchTypeSuccess(resultIsSuccess)
		{}
	}
}
