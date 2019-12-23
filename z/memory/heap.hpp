#pragma once
#include <stddef.h>
#include <mutex>
#include <type_traits>
#include <vector>

namespace z
{
	namespace memory
	{
		/**
		 * This class is for memory that is not meant to be freed throughout the
		 * lifetime of any usage of the memory. For example, a set of instructions
		 * in a program.
		 *
		 * \threadsafe_class_const
		 */
		class heap
		{
		private:
			unsigned char* mem;
			size_t memSize;
			size_t current;
			std::mutex memMutex; //protect memory from simultaneous calls to get()

			//Keep track of objects we will need to destroy later.
			struct destructor
			{
				const void* p;
				void(*destroy)(const void*);
			};
			std::vector<destructor> dtors;
			std::mutex dtorMutex; //protect from data races when adding destructors.

		public:
			///Allow allocated memory to increase if we run out of available space.
			bool increase;

			/**
			 * \brief Constructor. Initializes related heap data.
			 * \param canIncrease Whether to allow allocated memory to increase if we run out of space.
			 */
			heap(bool canIncrease = false);

			///Destructor
			~heap();

			/**
			 * \brief Reset heap and allocate a given number of bytes.
			 *
			 * Note that if the amount of memory we're allocating is smaller than what is already
			 * allocated, the heap does not resize (as we already have enough). Instead, the
			 * existing heap is reused.
			 *
			 * \warning Improper use of allocated memory can cause data corruptions or segmentation faults!
			 * It is up to you to ensure memory is no longer in use before reallocating the heap.
			 *
			 * \threadsafe_member_no
			 * \param bytes The number of bytes to allocate.
			 * \return True if allocation was successful, false otherwise.
			 */
			bool allocate(size_t bytes);

			/**
			 * \brief Reset heap and allocate a given number of objects.
			 *
			 * Note that if the amount of memory we're allocating is smaller than what is already
			 * allocated, the heap does not resize (as we already have enough). Instead, the
			 * existing heap is reused.
			 *
			 * \warning Improper use of allocated memory can cause data corruptions or segmentation faults!
			 * It is up to you to ensure memory is no longer in use before reallocating the heap.
			 *
			 * \threadsafe_member_no
			 * \param bytes The number of objects to allocate.
			 * \return True if allocation was successful, false otherwise.
			 */
			template<typename T>
			bool allocate(size_t count)
			{
				if (count < 0) return false;
				size_t bytes = sizeof(T) * count;
				return allocate(bytes);
			}

			/**
			 * \brief Get the number of bytes currently allocated.
			 * \return The number of bytes available for use.
			 */
			size_t max() const
			{
				return memSize;
			}

			/**
			 * \brief Get the number of objects currently allocated.
			 * \return The number of objects of T's size that are available for use (rounded down).
			 */
			template<typename T>
			size_t max() const
			{
				return memSize / sizeof(T);
			}

			/**
			 * \brief Get the number of bytes in use.
			 * \return The amount of used heap in bytes.
			 */
			size_t used() const
			{
				return current;
			}

			/**
			 * \brief Get the number of objects in use.
			 * \return The amount of used heap in objects of T's size (rounded down).
			 */
			template<typename T>
			size_t used() const
			{
				return current / sizeof(T);
			}

			/**
			 * \brief Grab some memory from the heap.
			 *
			 * Reserve some heap memory for external use. If this->increase is true then the heap
			 * will automatically increase to accomodate the requested amount. Otherwise, if
			 * more memory is requested than is available, then the request fails.
			 *
			 * \threadsafe_member_yes
			 * \param bytes The amount of memory requested in bytes.
			 * \return On success returns a pointer to the requested memory segment. Returns NULL on failure.
			 */
			unsigned char* get(size_t bytes);

			/**
			 * \brief Grab memory for some objects from the heap.
			 *
			 * Reserve some heap memory for external use. If this->increase is true then the heap
			 * will automatically increase to accomodate the requested amount. Otherwise, if
			 * more memory is requested than is available, then the request fails.
			 *
			 * \threadsafe_member_yes
			 * \param bytes The amount of memory requested in objects of T's size.
			 * \return On success returns a pointer to the requested memory segment. Returns NULL on failure.
			 */
			template<typename T>
			T* get(size_t count = 1)
			{
				size_t bytes = sizeof(T) * count;
				return (T*)get(bytes);
			}

			/**
			 * \brief Initialize an object stored on this heap.
			 *
			 * Constructs a single object using the given parameters, and assigns the
			 * destructor to run when the heap is destroyed or reallocated.
			 * Note that the object must belong to this heap to be managed in this way.
			 *
			 * \threadsafe_member_yes
			 * \param pointer Reference to the object we're constructing.
			 * \param args The list of arguments to pass to the constructor.
			 * \throws std::runtime_error if the object does not belong to this heap.
			 */
			template<typename T, typename ...Args>
			void init(T* pointer, Args&& ... args)
			{
				//only init object if it was allocated in this heap.
				//Otherwise error out.
				auto memPtr = (T*)mem;
				if((pointer < memPtr) || ((pointer + sizeof(T)) > (memPtr + current)))
				{
					throw std::runtime_error("Attempt to initialize an object that does not belong to this heap!");
				}

				//Pointer is OK, so initialize.
				new(pointer) T(std::forward<Args>(args) ...);

				//We don't want simultaneous calls to mess up calling destructors later on.
				const std::lock_guard<std::mutex> lock(dtorMutex);

				//Remember that we want to destroy the object later.
				dtors.push_back({
					pointer,
					[](const void* x)
					{
						static_cast<const T*>(x)->~T();
					}
				});
			}
		};
	}
}
