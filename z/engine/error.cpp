#include "error.h"

#include <z/core/string.h>

namespace z
{
	namespace engine
	{
		error::error(const errorLevel level,
					const core::string<Char>& inFile,
					const core::string<Char>& msg,
					const Int atLine,
					const bool isLine) :
			errLevel(level), errFile(inFile), errMsg(msg),
			errLine(atLine), errLineDef(isLine)
		{}

		Int error::line() const
		{
			return errLine;
		}

		const core::string<Char>& error::file() const
		{
			return errFile;
		}

		const core::string<Char>& error::message() const
		{
			return errMsg;
		}

		errorLevel error::level() const
		{
			return errLevel;
		}

		core::string<Char> error::levelString() const
		{
			switch errLevel
			{
			case LOW:
				return "LOW";
			case MED:
				return "MED";
			case HIGH:
				return "HIGH";
			case SEVERE:
				return "SEVERE";
			default:
				return "NO ERROR";
			}
		}

		bool error::lineSpec() const
		{
			return errLineDef;
		}

		bool error::instrSpec() const
		{
			return !errLineDef;
		}

		core::string<Char> error::fullMessage() const
		{
			core::string<Char> msg = "[";
			msg += levelString();
			msg += "] Error in ";

			if (errFile.length())
			{
				msg += errFile;
				msg += ", ";
			}

			msg += (errLineDef ? "line" : "instruction");

			msg += " ";
			msg += errLine;
			msg += ": ";

			msg += errMsg;

			return msg;
		}
	}
}
