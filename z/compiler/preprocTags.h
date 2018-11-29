#pragma once

#include <z/core/string.h>

namespace z
{
	namespace compiler
	{
		class preprocTags
		{
		public:
			virtual zstring directive() const
			{
				return zstring();
			}

			virtual zstring directend() const
			{
				return zstring();
			}

			virtual zstring commentStart() const
			{
				return zstring();
			}

			virtual zstring commentStop() const
			{
				return zstring();
			}

			virtual zstring commentLine() const
			{
				return zstring();
			}
		};
	}
}
