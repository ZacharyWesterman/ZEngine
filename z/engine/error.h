#pragma once
#ifndef ENGINE_ERROR_H_INCLUDED
#define ENGINE_ERROR_H_INCLUDED

#include <z/z.h>
#include "errorLevel.h"

namespace z
{
	namespace core
	{
		template <typename>
		class string;
	}

	namespace engine
	{
		class error
		{
		private:
			const errorLevel errLevel;
			const core::string<Char>* errFile;
			const core::string<Char>* errMsg;
			const Int errLine;
			const bool errLineDef;

		public:
			error(const errorLevel,
				const core::string<Char>*,
				const core::string<Char>*,
				const Int,
				const bool);

			Int line() const;

			const core::string<Char>& file() const;
			const core::string<Char>& message() const;

			errorLevel level() const;
			core::string<Char> levelString() const;

			bool lineSpec() const;
			bool instrSpec() const;

			core::string<Char> fullMessage() const;
		};
	}
}

#endif //ENGINE_ERROR_H_INCLUDED
