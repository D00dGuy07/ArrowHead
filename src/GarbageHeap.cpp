#include "oglfv2/Util/GarbageHeap.h"

constexpr size_t PointerBufScale = 512;

GarbageHeap::GarbageHeap(size_t size)
	: m_Buffer(malloc(size)), m_BufferCursor(m_Buffer), m_Pointers(malloc(size / PointerBufScale)), m_NumPointers(0) {}

GarbageHeap::~GarbageHeap()
{
	free(m_Buffer);
	free(m_Pointers);
}

void GarbageHeap::CleanupGarbage()
{
	for (uint32_t i = 0; i < m_NumPointers; i++)
		reinterpret_cast<BaseGarbagePointer*>(reinterpret_cast<GarbagePointer<void>*>(m_Pointers) + i)->~BaseGarbagePointer();
	m_BufferCursor = m_Buffer;
	m_NumPointers = 0;
	// The data stored in both of the buffers doesn't actually need to be 
	// cleared because it will just be overwritten
}

