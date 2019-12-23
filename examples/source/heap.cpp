#include <z/system/console.h>
#include <z/core/string.h>
#include <z/memory/heap.hpp>

void printHeapSpace(const z::memory::heap& heap)
{
	z::system::console console;

	zstring message = "We have space for ";
	message += heap.max<int>();
	message += " int";
	if (heap.max<int>() != 1) message += 's';
	message += " (";
	message += heap.max();
	message += " bytes).";

	message.writeln(console);
}

int main()
{
	z::system::console console;

	z::memory::heap heap;

	//Set up enough contiguous space for 1 int.
	//Note that if we try to reallocate to less than what we already have,
	//nothing will happen because we already have at least that much available.
	if (heap.allocate<int>(1))
	{
		(zstring("Allocated space for ") + heap.max<float>() + " int (" + heap.max() + " bytes).").writeln(console);
	}
	else
	{
		zstring("Allocation failed!").writeln(console);
		return 0;
	}

	auto reference = heap.get<int>();

	if (!reference)
	{
		zstring("Unable to get pointer for int!").writeln(console);
	}
	else
	{
		*reference = 1234;
		(zstring("Stored ") + (*reference) + " in the pointer we got.").writeln(console);
	}

	console.put('\n');
	int* newInts;

	//try to get 5 ints (will fail because heap is not allowed to resize)
	newInts = heap.get<int>(5);
	zstring(newInts ? "Successfully got 5 ints!" : "Unable to get memory for 5 ints!").writeln(console);
	printHeapSpace(heap);

	//allow heap to increase as needed
	heap.increase = true;
	console.put('\n');

	//Again, try to get 5 ints (this time will succeed and heap will increase)
	newInts = heap.get<int>(5);
	zstring(newInts ? "Successfully got 5 ints!" : "Unable to get memory for 5 ints!").writeln(console);
	printHeapSpace(heap);

	(zstring("\nWe've used ") + heap.used() + "/" + heap.max() + " bytes.").writeln(console);

	return 0;
}
