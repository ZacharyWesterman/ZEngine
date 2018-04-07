#pragma once

#ifndef SYMBOLRULE_H_INCLUDED
#define SYMBOLRULE_H_INCLUDED

namespace z
{
	namespace compiler
	{
		class symbolRule
		{
		public:
			const core::string<Char> pattern;
			const Int match_type;
			const Int match_sub_type;

			symbolRule(const core::string<Char>& Pattern, Int matchType, Int matchSubType = 0) :
				pattern(Pattern), match_type(matchType), match_sub_type(matchSubType) {}
			virtual ~symbolRule() {}

			virtual void onMatch(identity& ident) = 0;
		};
	}
}

#endif
