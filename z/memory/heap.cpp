#include <cstring>
#include "heap.hpp"

namespace z
{
	namespace memory
	{
		heap::heap(bool canIncrease) :
			mem(NULL),
			memSize(0),
			current(0),
			increase(canIncrease)
		{}

		heap::~heap()
		{
			for (auto& dtor : dtors)
			{
				dtor.destroy(dtor.p);
			}
			if (mem) delete mem;
		}

		bool heap::allocate(size_t bytes)
		{
			//call destructors of objects we're managing.
			for (auto& dtor : dtors)
			{
				dtor.destroy(dtor.p);
			}
			dtors.clear();

			//only reallocate if we have less than what we need.
			if (memSize < bytes)
			{
				if (mem) delete mem;
				mem = new unsigned char[bytes];
				memSize = bytes;
				current = 0;
				return (bool)mem;
			}

			return true;
		}

		unsigned char* heap::get(size_t bytes)
		{
			//We don't want simultaneous calls to mess up memory requests.
			const std::lock_guard<std::mutex> lock(memMutex);

			if (!(mem && bytes)) return NULL;

			size_t upcoming = current + bytes;
			if (upcoming > memSize)
			{
				//not enough contiguous space..
				if (increase) //so try to get more
				{
					//allocate just enough to get by.
					auto newMem = new unsigned char[upcoming];
					if (!newMem) return NULL;

					std::memcpy(newMem, mem, upcoming);
					memSize = upcoming;
					delete[] mem;
					mem = newMem;
				}
				else return NULL;
			}

			unsigned char* pointer = &mem[current];
			current = upcoming;
			return pointer;
		}

		void heap::reserveInit(int count)
		{
			//We don't want simultaneous calls to mess up calling destructors later on.
			const std::lock_guard<std::mutex> lock(dtorMutex);

			dtors.reserve(dtors.size() + count);
		}
	}
}
