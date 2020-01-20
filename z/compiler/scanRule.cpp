#include "scanRule.hpp"

namespace z
{
	namespace compiler
	{
		scanRule::scanRule(const zstring& pattern, int resultID, void (*resultOnMatch)(zstring&, scanner&)) :
			matchPattern(pattern),
			matchID(resultID),
			onMatch(resultOnMatch)
		{}
	}
}
