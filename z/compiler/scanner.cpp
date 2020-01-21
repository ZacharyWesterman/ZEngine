#include "scanner.hpp"
#include <z/system/console.hpp>
#include <z/core/memoryStream.hpp>

namespace z
{
	namespace compiler
	{
		scanner::scanner(z::core::outputStream& stream) : loggable(stream), currentScope(0) {}

		bool scanner::scanOnce(z::core::inputStream& stream)
		{
			column = (stream.tell() >> 2) + 1;

			for (auto& rule : rules[currentScope])
			{
				if (rule.matchPattern.match(stream))
				{
					auto text = rule.matchPattern.matched();

					//call the rule's special onMatch function if it exists
					if (rule.onMatch)
					{
						rule.onMatch(text, *this);
					}

					return true;
				}
			}

			return false;
		}

		void scanner::scan(z::core::inputStream& stream)
		{
			if (!stream.good()) return;

			line = column = 1;
			errorCount = warningCount = 0;

			thisLine.clear();
			while(!stream.empty())
			{
				thisLine.readln(stream);

				z::core::memoryStream lineStream (thisLine.wstring(), thisLine.length());
				lineStream.setFormat(z::utf32, true);

				zstring badText;

				while(!lineStream.empty())
				{
					bool matched = scanOnce(lineStream);

					//We didn't match any rule.. that's an error.
					if (!matched)
					{
						if (lineStream.empty()) break;
						badText.append(lineStream.getChar());
					}
					else
					{
						if (badText.length())
						{
							error(zstring("Invalid text \"")+badText+"\".");
							badText.clear();
						}
					}
				}

				if (badText.length()) error(zstring("Invalid text \"")+badText+"\".");

				++line;
			}
		}

		void scanner::addRule(scanRule&& rule, int scope)
		{
			if (rule.matchPattern.bad())
				error(zstring("Bad regex in scanner rule: ") + rule.matchPattern.errorString());
			else
				rules[scope].add(std::move(rule));
		}

		void scanner::push(int scope)
		{
			scopes.push(currentScope);
			currentScope = scope;
		}

		void scanner::pop()
		{
			if (scopes.empty())
			{
				currentScope = 0;
			}
			else
			{
				currentScope = scopes.top();
				scopes.pop();
			}
		}

	}
}
