#include "Arrow/Core/Arena.h"

#include "Arrow/Core/Logger.h"

#include <cstring>

constexpr size_t ScratchSize = 1024 * 1024;

namespace Arrow
{
	Arena::Arena(size_t size)
		: m_TotalSize(size)
	{
		// This constructor assumes that the data block is allocated right after the arena structure
		m_Data = reinterpret_cast<uint8_t*>(this + 1);
		m_Position = m_Data;
	}

	void* Arena::Push(size_t size)
	{
		// Check for overflow
		m_AllocatedSize += size;
		ARW_CORE_ASSERT(m_AllocatedSize < m_TotalSize, "Arena pushed out of bounds");

		// Increment internal pointer and return the block
		void* block = m_Position;
		m_Position += size;
		return block;
	}

	void Arena::Pop(size_t size)
	{
		// Prevent the position from 'popping' past the start
		size = std::min(size, m_AllocatedSize);
		m_AllocatedSize -= size;
		m_Position -= size;
	}

	void* Arena::PushZero(size_t size)
	{
		// Pretty self explanatory?
		void* block = Push(size);
		memset(block, 0, size);
		return block;
	}

	void Arena::SetPosBack(size_t pos)
	{
		// Prevent overflow
		ARW_CORE_ASSERT(pos < m_TotalSize, "Arena set pos must be inside the bounds");
		m_AllocatedSize = pos;
		m_Position = m_Data + pos;
	}

	void Arena::Clear()
	{
		m_AllocatedSize = 0;
		m_Position = m_Data;
	}

	// This function causes a buffer overun warning that shouldn't actually be a problem
#pragma warning( push )
#pragma warning( disable : 6386)
	Arena* Arena::Create(size_t size)
	{
		// Allocate the memory block with an arena structure at the beginning
		size_t bufferSize = sizeof(Arena) + size;
		void* buffer = std::malloc(bufferSize);
		return new(buffer) Arena(size);
	}
#pragma warning( pop )

	void Arena::InitScratch()
	{
		if (s_TempScratch == nullptr && s_PersistentScratch == nullptr)
		{
			s_TempScratch = Arena::Create(ScratchSize);
			s_PersistentScratch = Arena::Create(ScratchSize);
		}
	}


	ScratchSpace::ScratchSpace(Arena* arena)
		: m_Arena(arena), m_ResetPos(arena->GetPos()), m_HasReset(false) { }

	void ScratchSpace::Reset()
	{
		m_Arena->SetPosBack(m_ResetPos);
	}
}

