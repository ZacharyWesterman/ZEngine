#include <z/system/console.h>
using namespace z;

#include <z/util/regex.h>

#include <z/char.h>

#include "z/compiler/identity.h"

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

		class identifier : public symbolRule
		{
		public:
			identifier() : symbolRule("\\w+", 1) {}

			void onMatch(identity& ident)
			{

			}
		};
	}
}

int main(int argc, char* argv[])
{
	system::console console;

	z::compiler::symbolRule* rule = new z::compiler::identifier;

	// console.writeln(rule->pattern.match("1hello"));

	return 0;
}
