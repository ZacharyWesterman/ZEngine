#pragma once
#include <z/core/string.h>

namespace z
{
	namespace compiler
	{
		class loggable
		{
		private:
			int errorCount;
			int warningCount;

			zstring lineStandIn; //in case thisLine is null;

		protected:
			zstring* thisLine;

		public:
			int line;
			int column;
			zstring file;

			loggable();

			void error(const zstring& message);
			void warn(const zstring& message);
			void note(const zstring& message);
			void log(const zstring& message);
			void logLine(int length = 0);

			const zstring& currentLine() const;
		};
	}
}
