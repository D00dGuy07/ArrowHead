#pragma once

#include <vector>

namespace Sorting
{
	template<typename t>
	void MoveInsert(std::vector<t>& data, int32_t from, int32_t to)
	{
		t temp = data[to];
		data[to] = data[from];

		if (from > to)
		{
			for (int32_t i = to + 1; i <= from; i++)
			{
				t temp2 = temp;
				temp = data[i];
				data[i] = temp2;
			}
		}
		else if(to > from)
		{
			for (int32_t i = to - 1; i >= from; i--)
			{
				t temp2 = temp;
				temp = data[i];
				data[i] = temp2;
			}
		}
	}

	enum class Algorithm
	{
		Insertion,
		Quick,
		Heap,
		HeapBottomUp,
		Introsort
	};

	// Insertion sort
	template<typename t, class c>
	void SortInsertion(std::vector<t>& data, int32_t begin, int32_t end, c compare)
	{
		int32_t tempIndex = 0;
		for (int32_t i = begin + 1; i <= end; i++)
		{
			tempIndex = i - 1;
			if (compare(data[tempIndex], data[i]))
			{
				t key = data[i];
				data[i] = data[tempIndex];

				tempIndex--;
				while (tempIndex >= begin && compare(data[tempIndex], key)) 
				{ 
					data[tempIndex + 1] = data[tempIndex];
					tempIndex--;
				}
				data[tempIndex + 1] = key;
			}
		}
	}

	// Quick sort (https://www.geeksforgeeks.org/quick-sort/)
	template<typename t, class c>
	int32_t SortQuickPartition(std::vector<t>& data, int32_t begin, int32_t end, c compare)
	{
		t pivot = data[end];
		int32_t i = (begin - 1);

		for (int32_t j = begin; j < end; j++)
		{
			if (compare(pivot, data[j]))
			{
				i++;
				t temp = data[i];
				data[i] = data[j];
				data[j] = temp;
			}
		}

		t temp = data[i + 1];
		data[i + 1] = data[end];
		data[end] = temp;
		return i + 1;
	}

	template<typename t, class c>
	void SortQuick(std::vector<t>& data, int32_t begin, int32_t end, c compare)
	{
		if (begin < end)
		{
			int32_t pi = SortQuickPartition<t>(data, begin, end, compare);

			SortQuick<t>(data, begin, pi - 1, compare);
			SortQuick<t>(data, pi + 1, end, compare);
		}
	}

	// Heap Sort
	template<typename t, class c>
	void SiftDown(std::vector<t>& data, int32_t begin, int32_t end, c compare)
	{
		int32_t root = begin;
		while (2 * root + 1 <= end)
		{
			int32_t child = 2 * root + 1;
			int32_t swap = root;

			if (compare(data[child], data[swap]))
				swap = child;
			if (child + 1 <= end && compare(data[child + 1], data[swap]))
				swap = child + 1;
			if (swap == root)
				return;
			else
			{
				t temp = data[root];
				data[root] = data[swap];
				data[swap] = temp;

				root = swap;
			}
		}
	}

	template<typename t, class c>
	void Heapify(std::vector<t>& data, int32_t begin, int32_t end, c compare)
	{
		for (int32_t i = (end - 1) / 2; i >= begin; i--)
			SiftDown<t>(data, i, end, compare);
	}

	template<typename t, class c>
	void SortHeap(std::vector<t>& data, int32_t begin, int32_t end, c compare)
	{
		Heapify<t>(data, begin, end, compare);

		int32_t heapEnd = end;
		while (heapEnd > begin)
		{
			t temp = data[heapEnd];
			data[heapEnd] = data[begin];
			data[begin] = temp;

			heapEnd--;

			SiftDown<t>(data, begin, heapEnd, compare);
		}
	}

	// Heap Sort Bottom Up

	template<typename t, class c>
	int32_t LeafSearch(std::vector<t>& data, int32_t begin, int32_t end, c compare)
	{
		int32_t j = begin;
		while (2 * j + 2 <= end)
		{
			if (compare(data[2 * j + 2], data[2 * j + 1]))
				j = 2 * j + 2;
			else
				j = 2 * j + 1;
		}
		if (2 * j + 1 <= end)
			j = 2 * j + 1;
		return j;
	}

	template<typename t, class c>
	void SiftDownBottomUp(std::vector<t>& data, int32_t begin, int32_t end, c compare)
	{
		int32_t j = LeafSearch<t>(data, begin, end, compare);
		while (compare(data[begin], data[j]))
			j = (j - 1) / 2;
		t x = data[j];
		data[j] = data[begin];
		while (j > begin)
		{
			t temp = x;
			x = data[(j - 1) / 2];
			data[(j - 1) / 2] = temp;
			j = (j - 1) / 2;
		}
	}

	template<typename t, class c>
	void HeapifyBottomUp(std::vector<t>& data, int32_t begin, int32_t end, c compare)
	{
		for (int32_t i = (end - 1) / 2; i >= begin; i--)
			SiftDownBottomUp<t>(data, i, end, compare);
	}

	template<typename t, class c>
	void SortHeapBottomUp(std::vector<t>& data, int32_t begin, int32_t end, c compare)
	{
		HeapifyBottomUp<t>(data, begin, end, compare);

		int32_t heapEnd = end;
		while (heapEnd > begin)
		{
			t temp = data[heapEnd];
			data[heapEnd] = data[begin];
			data[begin] = temp;

			heapEnd--;

			SiftDownBottomUp<t>(data, begin, heapEnd, compare);
		}
	}

	// Introsort

	template<typename t, class c>
	void IntrosortRecurse(std::vector<t>& data, int32_t begin, int32_t end, int32_t depthLimit, c compare)
	{
		int32_t size = end - begin + 1;

		if (size < 16)
		{
			SortInsertion(data, begin, end, compare);
			return;
		}

		if (depthLimit == 0)
		{
			SortHeapBottomUp(data, begin, end, compare);
			return;
		}

		int32_t pivotIndex = begin;
		{
			// Find median of three
			// I could put this in it's own function but that feels unnecessary atm

			int32_t a = begin;
			int32_t b = begin + size / 2;
			int32_t _c = end;

			if (compare(data[a], data[b]))
			{
				int32_t temp = a;
				a = b;
				b = temp;
			}
			if (compare(data[b], data[_c]))
			{
				int32_t temp = b;
				b = _c;
				_c = temp;

				if (compare(data[a], data[b]))
				{
					int32_t temp = a;
					a = b;
					b = temp;
				}
			}

			pivotIndex = b;
		}

		t temp = data[pivotIndex];
		data[pivotIndex] = data[end];
		data[end] = temp;

		// Sort recursively
		int32_t pi = SortQuickPartition<t>(data, begin, end, compare);
		IntrosortRecurse<t>(data, begin, pi - 1, depthLimit - 1, compare);
		IntrosortRecurse<t>(data, pi + 1, end, depthLimit - 1, compare);
	}

	template<typename t, class c>
	void SortIntro(std::vector<t>& data, int32_t begin, int32_t end, c compare)
	{
		int32_t depthLimit = 2 * static_cast<int32_t>(std::log(end - begin + 1));
		IntrosortRecurse<t>(data, begin, end, depthLimit, compare);
	}

	template<Algorithm a, typename t, class c>
	void Sort(std::vector<t>& data, int32_t begin, int32_t end, c compare)
	{
		switch (a)
		{
		case Algorithm::Insertion:
			SortInsertion(data, begin, end, compare);
			break;
		case Algorithm::Quick:
			SortQuick(data, begin, end, compare);
			break;
		case Algorithm::Heap:
			SortHeap(data, begin, end, compare);
			break;
		case Algorithm::HeapBottomUp:
			SortHeapBottomUp(data, begin, end, compare);
			break;
		case Algorithm::Introsort:
			SortIntro(data, begin, end, compare);
			break;
		default:
			break;
		}
	}
}