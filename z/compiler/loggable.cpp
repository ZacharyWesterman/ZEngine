#include "loggable.hpp"
#include <z/system/console.h>

namespace z
{
	namespace compiler
	{
		loggable::loggable() :
			thisLine(NULL),
			errorCount(0),
			warningCount(0)
		{}

		void loggable::error(const zstring& message)
		{
			z::system::console console;

			zstring msg;
			if (console.istty()) msg = "\u001b[1m";
			msg += file;
			msg += ':';
			msg += line;
			msg += ':';
			msg += column;
			msg += ": ";
			if (console.istty()) msg += "\u001b[38;5;196m";
			msg += "error:";
			if (console.istty()) msg += "\u001b[0m";
			msg += ' ';
			msg += message;
			msg.writeln(console);

			++errorCount;
		}

		void loggable::warn(const zstring& message)
		{
			z::system::console console;

			zstring msg;
			if (console.istty()) msg = "\u001b[1m";
			msg += file;
			msg += ':';
			msg += line;
			msg += ':';
			msg += column;
			msg += ": ";
			if (console.istty()) msg += "\u001b[38;5;140m";
			msg += "warning:";
			if (console.istty()) msg += "\u001b[0m";
			msg += ' ';
			msg += message;
			msg.writeln(console);

			++warningCount;
		}

		void loggable::note(const zstring& message)
		{
			z::system::console console;

			zstring msg;
			if (console.istty()) msg = "\u001b[1m\u001b[38;5;81m";
			msg += "note:";
			if (console.istty()) msg += "\u001b[0m";
			msg += ' ';
			msg += message;
			msg.writeln(console);
		}

		void loggable::log(const zstring& message)
		{
			z::system::console console;
			message.writeln(console);
		}

		void loggable::logLine(int length)
		{
			z::system::console console;

			zstring current = currentLine();
			zstring msg = zstring("    ") + current.trimLeft();

			int pos = column + msg.length() - current.length() - 1;

			msg.writeln(console);

			msg.clear();
			msg.repeat(' ', pos);
			if (console.istty()) msg += "\u001b[1m\u001b[38;5;118m";
			msg += '^';
			if (length > 2) msg.repeat(L'-',length-2);
			if (length > 1) msg += '^';
			if (console.istty()) msg += "\u001b[0m";

			msg.writeln(console);
		}

		const zstring& loggable::currentLine() const
		{
			if (thisLine) return *thisLine;
			return lineStandIn;
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
