#pragma once
#include <z/core/array.hpp>
#include <z/core/stream.hpp>

#include "scanRule.hpp"
#include "loggable.hpp"

#include <map>
#include <stack>

namespace z
{
	namespace compiler
	{
		class scanner : public loggable
		{
		private:
			bool scanOnce(z::core::inputStream& stream);
			std::map<int, z::core::array<scanRule>> rules;

			std::stack<int> scopes;
			int currentScope;

		public:
			scanner(z::core::outputStream& stream);

			void scan(z::core::inputStream& stream);
			void addRule(scanRule&& rule, int scope = 0);

			void push(int scope);
			void pop();
		};
	}
}
