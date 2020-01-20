#include "loggable.hpp"

namespace z
{
	namespace compiler
	{
		loggable::loggable(z::core::outputStream& stream) :
			output(stream),
			errorCount(0),
			warningCount(0)
		{}

		void loggable::error(const zstring& message)
		{
			zstring msg;
			if (output.istty()) msg = "\u001b[1m";
			if (file.length())
			{
				msg += file;
				msg += ':';
				msg += line;
				msg += ':';
				msg += column;
				msg += ": ";
			}
			if (output.istty()) msg += "\u001b[38;5;196m";
			msg += "error:";
			if (output.istty()) msg += "\u001b[0m";
			msg += ' ';
			msg += message;
			msg.writeln(output);

			++errorCount;
		}

		void loggable::warn(const zstring& message)
		{
			zstring msg;
			if (output.istty()) msg = "\u001b[1m";
			if (file.length())
			{
				msg += file;
				msg += ':';
				msg += line;
				msg += ':';
				msg += column;
				msg += ": ";
			}
			if (output.istty()) msg += "\u001b[38;5;140m";
			msg += "warning:";
			if (output.istty()) msg += "\u001b[0m";
			msg += ' ';
			msg += message;
			msg.writeln(output);

			++warningCount;
		}

		void loggable::note(const zstring& message)
		{
			zstring msg;
			if (output.istty()) msg = "\u001b[1m\u001b[38;5;81m";
			msg += "note:";
			if (output.istty()) msg += "\u001b[0m";
			msg += ' ';
			msg += message;
			msg.writeln(output);
		}

		void loggable::log(const zstring& message)
		{
			message.writeln(output);
		}

		void loggable::logLine(int length)
		{
			zstring current = currentLine();
			zstring msg = zstring("    ") + current.trimLeft();

			int pos = column + msg.length() - current.length() - 1;

			msg.writeln(output);

			msg.clear();
			msg.repeat(' ', pos);
			if (output.istty()) msg += "\u001b[1m\u001b[38;5;118m";
			msg += '^';
			if (length > 2) msg.repeat(L'-',length-2);
			if (length > 1) msg += '^';
			if (output.istty()) msg += "\u001b[0m";

			msg.writeln(output);
		}

		const zstring& loggable::currentLine() const
		{
			return thisLine;
		}

		int loggable::errors() const
		{
			return errorCount;
		}

		int loggable::warnings() const
		{
			return warningCount;
		}
	}
}
