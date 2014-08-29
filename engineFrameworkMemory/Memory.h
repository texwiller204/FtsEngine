//
// Memory:
// This file contains implemenation for the GDK memory system.  Useable in GDK components & games
//
// Configuration:
//
// Usage:
//              Single object
//                      Foo *foo = GdkNew Foo();
//                      GdkDelete foo;
//              1D Array of created objects
//                      Foo **foo = GdkNew1DArray<Foo>(10);
//                      GdkDelete1DArray foo;
//              2D Array of created objects
//                      Foo ***foo = GdkNew2DArray<Foo>(10,20);
//                      GdkDelete2DArray foo;
//              malloc & free
//                      Byte* buffer = GdkAlloc(100);
//                      GdkFree(buffer);
//
//

//// Init the GDK Memory
//Gdk::Memory::Init();
//
//Foo *foo = GdkNew Foo();
////GdkDelete<Foo>(foo);
////    1D Array of created objects
//Foo *foo1D = GdkNew1DArray<Foo>(10);
////GdkDelete1DArray<Foo>(foo1D);
//
//////    2D Array of created objects
//Foo **foo2D = GdkNew2DArray<Foo>(10, 20);
////GdkDelete2DArray<Foo>(foo2D);
//
////     malloc & free
//Byte* buffer = (Byte*)GdkAlloc(100);
//GdkFree(buffer);

#pragma once

#include "GdkLIB.h"
#include "Assert.h"

// Comment this line to disable memory tracking
#if defined(DEBUG) || defined(_DEBUG)
	#define GDK_MEMORY_TRACKING
#endif



// ---------------------
#ifdef GDK_MEMORY_TRACKING

namespace Gdk
{

	class Memory
	{
		// internal Typedefs & Classes
		// -----------------------------------

		// Allocator functions types  (exposed as function pointers, so they can be customized by the user)
	public:
		typedef void* (*AllocatorFunc)(size_t numBytes, const char* file, int line);
		typedef void(*DeallocatorFunc)(void* memblock, const char* file, int line);

		// MemoryEntry class:  used to represent data for a single memory allocation
	private:
		class MemoryEntry
		{
		public:
			inline MemoryEntry() {}
			inline MemoryEntry(int numBytes, int numDimensions, const char* file, int line)
				:
				numBytes(numBytes),
				numDimensions(numDimensions),
				file(file),
				line(line),
				uniqueId(++uniqueIdCounter)
			{
			}

			int numBytes;
			int numDimensions;
			const char* file;
			int line;
			unsigned int uniqueId;
			static unsigned int uniqueIdCounter;
		};

		typedef std::map<void*, MemoryEntry> MemoryMap;
		typedef std::map<unsigned int, std::pair<void*, MemoryEntry> > SortedMap;

		// Static Properties & Methods
		// -------------------------------------

	//	// Init & Shutdown - called by the Application
	//private:
	//	friend class Application;
	//	static void Init();
	//	static void Shutdown();

	public:
		static void Init();
		static void Shutdown();


		// Standard C malloc/free pass-through allocators
	private:
		static void* CAllocator(size_t numBytes, const char* file, int line);
		static void CDeallocator(void* memBlock, const char* file, int line);

		// Properties
	private:
		static MemoryMap* memoryMap;
		static AllocatorFunc allocatorFunc;
		static DeallocatorFunc deallocatorFunc;

		// TODO(P2): Mutex locked thread safe allocations...  static Mutex mutex;

		typedef std::unique_lock<std::mutex> ScopeLock;

		
		// Instance Properties & Methods
		// -------------------------------------

		// Constructor
	public:
		inline Memory(const char* file, int line) : file(file), line(line) {}
		inline ~Memory() {}

		// Memory allocation
		void* CreateBlock(size_t numBytes, int numDimensions) const;

		// For 1D arrays:  data[bound0]
		template <typename T> T* New1DArray(const int bound0);

		// For 2D arrays:  data[bound1][bound0]
		template <typename T> T** New2DArray(const int bound0, const int bound1);

		// For singletons.
		template <typename T> void Delete(T*& data);

		// For 1D arrays:  data[bound0]
		template <typename T> void Delete1DArray(T*& data);

		// For 2D arrays:  data[bound1][bound0]
		template <typename T> void Delete2DArray(T**& data);

		// C-style alloc & free
		void* Alloc(size_t numBytes);
		void Free(void* ptr);

	private:
		// Instance properties
		const char* file;
		int line;
		//ScopeLock	lock;
		mutable std::mutex memLock;
	};

#include "Memory.inl"

}

//----------------------------------------------------------------------------
// Operator:  new (Memory&)
inline void* operator new (size_t numBytes, const Gdk::Memory& memory)
{
	return memory.CreateBlock(numBytes, 0);
}
//----------------------------------------------------------------------------
// Operator: delete (Memory&)
inline void operator delete (void*, const Gdk::Memory&)
{
	// Only called during exception handling.
}

//----------------------------------------------------------------------------

// Allocation Macros (for tracked memory)
#define GdkNew                  new(Gdk::Memory(__FILE__,__LINE__))
#define GdkNew1DArray           Gdk::Memory(__FILE__,__LINE__).New1DArray
#define GdkNew2DArray           Gdk::Memory(__FILE__,__LINE__).New2DArray
#define GdkDelete               Gdk::Memory(__FILE__,__LINE__).Delete
#define GdkDelete1DArray        Gdk::Memory(__FILE__,__LINE__).Delete1DArray
#define GdkDelete2DArray        Gdk::Memory(__FILE__,__LINE__).Delete2DArray
#define GdkAlloc(size)          Gdk::Memory(__FILE__,__LINE__).Alloc(size)
#define GdkFree(ptr)            Gdk::Memory(__FILE__,__LINE__).Free(ptr)

#else // ifdef GDK_MEMORY_TRACKING

namespace Gdk
{
	class Memory
	{
	public:
		// Init & Shutdown - called by the Application
		static void Init();
		static void Shutdown();
	};
}

// Allocation Macros (pass-throughs)
#define GdkNew new
#define GdkAlloc(size) malloc(size)
#define GdkFree(ptr)   free(ptr)

// For 1D arrays:  data[bound0]
template <typename T>
T* GdkNew1DArray(const int bound0);

// For 2D arrays:  data[bound1][bound0]
template <typename T>
T** GdkNew2DArray(const int bound0, const int bound1);

// For singletons.
template <typename T>
void GdkDelete(T*& data);

// For 1D arrays:  data[bound0]
template <typename T>
void GdkDelete1DArray(T*& data);

// For 2D arrays:  data[bound1][bound0]
template <typename T>
void GdkDelete2DArray(T**& data);

#include "Memory.inl"


#endif // ifdef GDK_MEMORY_TRACKING
