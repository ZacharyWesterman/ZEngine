#pragma once
#include <z/core/sortedRefArray.hpp>
#include <z/core/stream.hpp>

#include "scanRule.hpp"
#include "loggable.hpp"
#include "identifier.hpp"

#include <map>
#include <stack>

namespace z
{
	namespace compiler
	{
		class scanner : public loggable
		{
		private:
			bool scanOnce(
				z::core::inputStream& stream,
				z::core::sortedRefArray<zstring*>& symbols,
				z::core::array<identifier>& idents,
				zstring* fileName
			);

			std::map<int, z::core::array<scanRule>> rules;

			std::stack<int> scopes;
			int currentScope;

		public:
			scanner(z::core::outputStream& stream);

			z::core::array<identifier> scan(z::core::inputStream& stream, z::core::sortedRefArray<zstring*>& symbols);
			void addRule(scanRule&& rule, int scope = 0);

			void push(int scope);
			void pop();
		};
	}
}
