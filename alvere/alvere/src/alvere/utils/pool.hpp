#pragma once

#include <vector>
#include <limits>

namespace alvere
{
	template <typename T>
	class Pool
	{
		std::vector<T> m_Elements;
		std::vector<bool> m_Allocated;
		std::vector<std::size_t> m_FreeList;
		std::vector<unsigned> m_Versions;

		std::size_t m_FirstFree;

	public:

		class Handle;

		Pool();
		Pool(int initialCapacity);

		template <typename... Args>
		Pool<T>::Handle allocate(Args &&... args)
		{
			if (m_FirstFree == std::numeric_limits<std::size_t>::max())
			{
				m_Allocated.emplace_back(true);
				m_FreeList.emplace_back(std::numeric_limits<std::size_t>::max());
				m_Elements.emplace_back(std::forward<Args>(args)...);
				m_Versions.emplace_back(0);
				return Handle(*this, m_Elements.size() - 1);
			}

			std::size_t index = m_FirstFree;

			m_FirstFree = m_FreeList[index];
			m_Allocated[index] = true;
			m_Versions[index] += 1;
			m_FreeList[index] = std::numeric_limits<std::size_t>::max();
			T & element = *new(&m_Elements[index]) T(std::forward<Args>(args)...);

			return Handle(*this, index);
		}

		void deallocate(Pool<T>::Handle & handle)
		{
			m_Elements[handle.m_Index].~T();
			m_Allocated[handle.m_Index] = false;
			m_FreeList[handle.m_Index] = m_FirstFree;
			m_Versions[handle.m_Index] += 1;
			m_FirstFree = handle.m_Index;

			handle.m_Index = std::numeric_limits<std::size_t>::max();
		}

	private:

		T & get(std::size_t index);
		int version(std::size_t index) const;
	};
}

#include "pool_handle.hpp"

namespace alvere
{
	template <typename T>
	Pool<T>::Pool()
		: m_FirstFree(std::numeric_limits<std::size_t>::max())
	{
	}

	template <typename T>
	Pool<T>::Pool(int initialCapacity)
		: m_FirstFree(std::numeric_limits<std::size_t>::max())
	{
		m_Elements.reserve(initialCapacity);
		m_Allocated.reserve(initialCapacity);
		m_FreeList.reserve(initialCapacity);
		m_Versions.reserve(initialCapacity);
	}

	template <typename T>
	T & Pool<T>::get(std::size_t index)
	{
		return m_Elements[index];
	}

	template <typename T>
	int Pool<T>::version(std::size_t index) const
	{
		return m_Versions[index];
	}
}