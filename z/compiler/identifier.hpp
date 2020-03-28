#pragma once

#include <z/core/string.hpp>

namespace z
{
	namespace compiler
	{
		class identifier
		{
		public:
			int id;
			int line;
			int column;
			zstring* text;
			zstring* file;

			identifier(){}

			identifier(int id, int line, int column, zstring* text, zstring* file) :
				id(id),
				line(line),
				column(column),
				text(text),
				file(file)
			{}
		};
	}
}
