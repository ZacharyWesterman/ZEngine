#include "preprocessor.hpp"
#include <z/file/inputStream.hpp>
#include <z/system/stdout.hpp>
#include <z/file/exists.hpp>

namespace grammar
{
	static void stripComment(zstring& line) noexcept
	{
		//strip away comments
		bool inQuote = false;
		int commentPos = -1;
		bool escaped = false;
		for (int i=0; i<line.length(); ++i)
		{
			auto chr = line[i];

			if (chr == '\\')
			{
				escaped = !escaped;
			}
			else if (!escaped)
			{
				if (chr == '\'')
				{
					inQuote = !inQuote;
				}
				else if (!inQuote && (chr == '#'))
				{
					commentPos = i;
					break;
				}
				escaped = false;
			}
		}
		if (commentPos > -1) line.remove(commentPos, line.length());
	}

	static zstring destringify(const zstring& text) noexcept
	{
		const int start = (text[0] == '\'');
		const int stop = text.length() - ((text[text.length()-1] == '\'') && (text[text.length()-2] != '\\'));
		zstring result;
		result.increase(text.length());

		bool escaped = false;
		for (int i=start; i<stop; ++i)
		{
			if (!escaped && (text[i] == '\\'))
			{
				escaped = true;
				continue;
			}

			result.append(text[i]);
			escaped = false;
		}
		if (escaped) result.append((uint32_t)'\\');

		return result;
	}

	bool preprocess(const zstring& filename, z::core::outputStream& output) noexcept
	{
		if (output.bad() || !z::file::exists(filename)) return false;
		z::file::inputStream input (filename);

		zstring dir = filename;
		dir.replace("\\","/");
		(zstring("~$")+dir).writeln(output); //specify what file we're in, for error reporting purposes.
		int pos = dir.findLast("/");
		if (pos > -1)
			dir.remove(pos+1, dir.length());
		else
			dir.clear();

		zstring line;
		do
		{
			line.readln(input);
			stripComment(line);

			line.trimIn();
			if (line[0] == '!') //include file, erroring out if it doesn't exist.
			{
				line.remove(0,1);
				line.trimLeftIn();
				line = dir+destringify(line);
				process(line, output);
				line = zstring("~$")+filename;
			}

			if (line.length()) line.writeln(output);
		}
		while (!input.empty());

		return true;
	}
}
