#pragma once

#include <iostream>

class GarbageHeap
{
public:
	GarbageHeap(size_t size);
	~GarbageHeap();

	struct BaseGarbagePointer
	{
		virtual ~BaseGarbagePointer() {}
	};

	template<typename T>
	struct GarbagePointer : public BaseGarbagePointer
	{
		T* Pointer;

		GarbagePointer(T* pointer)
			: Pointer(pointer) {}

		virtual ~GarbagePointer() override
		{
			Pointer->~T();
		}
	};

	template<typename T>
	T* Allocate()
	{
		// Get pointer of offset into m_Pointers array
		BaseGarbagePointer* pointer = 
			reinterpret_cast<BaseGarbagePointer*>(reinterpret_cast<GarbagePointer<void>*>(m_Pointers) + m_NumPointers);

		// Get pointer of the aligned memory block getting allocated
		T* memBlock = reinterpret_cast<T*>(((reinterpret_cast<uint64_t>(m_BufferCursor) + alignof(T) - 1) / alignof(T)) * alignof(T));

		// Create a new garbage collection helper
		new(pointer) GarbagePointer<T>(memBlock);

		// Move buffer cursor to past the allocated block
		m_BufferCursor = reinterpret_cast<void*>(reinterpret_cast<uint64_t>(memBlock) + sizeof(T));

		m_NumPointers++;
		return memBlock;
	}

	void CleanupGarbage();

private:
	void* m_Buffer;
	void* m_BufferCursor;

	void* m_Pointers;
	uint32_t m_NumPointers;
};