#pragma once

#include <z/core/array.h>
#include <z/util/generic.h>

namespace engine
{
	class heap
	{
	private:
		z::core::array< z::util::generic* > memory;

	public:
		heap(z::util::generic* callArg = NULL);
		~heap();

		void increase(size_t amount);
		void decrease(size_t amount);

		z::util::generic* get(size_t index);
		bool set(size_t index, z::util::generic* newValue);

		z::util::generic* args();
		z::util::generic* returnVal();

		size_t count() const;
	};
}
