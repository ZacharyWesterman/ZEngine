#include <z/system/console.h>
using namespace z;

#include <z/util/regex.h>

#include <z/char.h>

#include "z/compiler/identity.h"
#include "z/compiler/symbolRule.h"

#include <z/core.h>

// namespace z
// {
// 	namespace compiler
// 	{
// 		class identifier : public symbolRule
// 		{
// 		public:
// 			identifier() : symbolRule("\".*((?<!\\)\")", 1) {}
//
// 			void onMatch(identity& ident)
// 			{
//
// 			}
// 		};
// 	}
// }

int main(int argc, char* argv[])
{
	system::console console;

	core::string<char> string ("\".*((?!<\\\\)\"|[\\n$])");

	util::regex<char> regex(string);

	if (regex.bad())
		console.writeln("Invalid regex formatting.");

	core::stringStream<char> stream (cs("the following \"this is \\\"A string\\\", \"thank you."));

	Int found = regex.search(stream);

	if (found > 0)
	{
		auto msg = cs("Found ")+regex.foundLen()+" characters at index ";
		msg += cs(regex.foundAt())+" in input. (\"";
		msg += regex.foundString()+"\")";
		console.writeln(msg);
	}
	else
		console.writeln("No string found matching regex.");

	// z::compiler::symbolRule* rule = new z::compiler::identifier;

	// console.writeln(rule->pattern.match("1hello"));

	return 0;
}
