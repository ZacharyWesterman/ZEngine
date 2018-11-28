#pragma once

#include <z/core/string.h>

namespace z
{
	namespace compiler
	{
		class preprocTags
		{
		public:
			virtual core::string<> directive() const
			{
				return core::string<>();
			}

			virtual core::string<> directend() const
			{
				return core::string<>();
			}

			virtual core::string<> commentStart() const
			{
				return core::string<>();
			}

			virtual core::string<> commentStop() const
			{
				return core::string<>();
			}

			virtual core::string<> commentLine() const
			{
				return core::string<>();
			}
		};
	}
}
