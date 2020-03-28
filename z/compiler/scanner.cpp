#include "scanner.hpp"
#include <z/system/console.hpp>
#include <z/core/memoryStream.hpp>

namespace z
{
	namespace compiler
	{
		scanner::scanner(z::core::outputStream& stream) :
			loggable(stream),
			currentScope(0)
		{}

		bool scanner::scanOnce(
			z::core::inputStream& stream,
			z::core::sortedRefArray<zstring*>& symbols,
			z::core::array<identifier>& idents,
			zstring* fileName
		)
		{
			column = (stream.tell() >> 2) + 1;

			for (auto& rule : rules[currentScope])
			{
				if (rule.matchPattern.match(stream))
				{
					zstring* text = new zstring(rule.matchPattern.matched());

					//call the rule's special onMatch function if it exists
					if (rule.onMatch)
					{
						rule.onMatch(*text, *this);
					}

					//add text to symbol table if not already in there (saves on memory usage)
					auto pos = symbols.findInsert(text, false);
					if (pos >= 0)
					{
						symbols.insert(text, pos);
					}
					else
					{
						delete text;
						text = symbols[pos];
					}

					//add to list of identifiers
					idents.add(identifier(rule.matchID, line, column, text, fileName));

					return true;
				}
			}

			return false;
		}

		z::core::array<identifier> scanner::scan(z::core::inputStream& stream, z::core::sortedRefArray<zstring*>& symbols)
		{
			z::core::array<identifier> idents;

			if (!stream.good()) return idents;

			line = column = 1;
			errorCount = warningCount = 0;

			//add the file name to the symbol table
			zstring* fileName;
			auto pos = symbols.findInsert(&(this->file), false);
			if (pos >= 0)
			{
				fileName = new zstring(this->file);
				symbols.insert(fileName, pos);
			}
			else
			{
				fileName = symbols[pos];
			}

			thisLine.clear();
			while(!stream.empty())
			{
				thisLine.readln(stream);

				z::core::memoryStream lineStream (thisLine.wstring(), thisLine.length());
				lineStream.setFormat(z::utf32, true);

				zstring badText;

				while(!lineStream.empty())
				{
					bool matched = scanOnce(lineStream, symbols, idents, fileName);

					//We didn't match any rule.. that's an error.
					if (!matched && lineStream.empty()) break;

					auto strpos = lineStream.tell();
					auto ch = lineStream.getChar();
					if (z::core::isWhiteSpace(ch)) matched = true; //Ignore white space by default.

					if (!matched)
					{
						badText.append(ch);
					}
					else
					{
						if (!lineStream.empty()) lineStream.seek(strpos);
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

			return idents;
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
