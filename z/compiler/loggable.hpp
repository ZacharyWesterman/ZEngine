#pragma once
#include <z/core/string.hpp>
#include <z/core/stream.hpp>

namespace z
{
	namespace compiler
	{
		class loggable
		{
		protected:
			z::core::outputStream& output;
			int errorCount;
			int warningCount;
			zstring thisLine;

		public:
			int line;
			int column;
			zstring file;

			loggable(z::core::outputStream& stream);

			void error(const zstring& message);
			void warn(const zstring& message);
			void note(const zstring& message);
			void log(const zstring& message);
			void logLine(int length = 0);

			const zstring& currentLine() const;

			int errors() const;
			int warnings() const;
		};
	}
}
