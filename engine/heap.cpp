#include "heap.hpp"

namespace engine
{
	heap::heap(z::util::generic* callArg)
	{
		memory.add(new z::util::genericNull());
		if (callArg)
		{
			memory.add(callArg);
		}
		else
		{
			z::core::array< z::util::generic* > args;
			memory.add(new z::util::genericArray(args));
		}
		memory.add(new z::util::genericNull());
	}

	heap::~heap()
	{
		for (auto& slot : memory) delete slot;
	}

	void heap::increase(size_t amount)
	{
		for (size_t i=0; i<amount; ++i)
			memory.add(new z::util::genericNull());
	}

	void heap::decrease(size_t amount)
	{
		auto memsize = memory.length();
		if ((memsize <= 3) || !amount) return;

		size_t start;
		if ((amount+3) >= memsize)
			start = 3;
		else
			start = memsize - amount;

		for (size_t i=start; i<memsize; ++i) delete memory[i];
		memory.remove(start,amount);
	}

	z::util::generic* heap::get(size_t index)
	{
		size_t mempos = index >> 1;
		bool fromEnd = index & 1;
		auto memsize = memory.length();

		if (fromEnd) mempos = memsize - 1 - mempos;
		if (mempos >= memsize) return NULL;
		return memory[mempos];
	}

	bool heap::set(size_t index, z::util::generic* newValue)
	{
		size_t mempos = index >> 1;
		bool fromEnd = index & 1;
		auto memsize = memory.length();

		if (fromEnd) mempos = memsize - 1 - mempos;
		if (!mempos) return true;
		if (mempos >= memsize) return false;

		delete memory[mempos];
		memory[mempos] = newValue;
		return true;
	}

	z::util::generic* heap::args()
	{
		return memory[1];
	}

	z::util::generic* heap::returnVal()
	{
		return memory[2];
	}

	size_t heap::count() const
	{
		return memory.length();
	}
}
