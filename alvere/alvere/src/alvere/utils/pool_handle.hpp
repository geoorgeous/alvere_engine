#pragma once

#include <limits>

#include "alvere/utils/exceptions.hpp"
#include "alvere/utils/pool.hpp"

namespace alvere
{
	template <typename T>
	class Pool<T>::Handle
	{
		Pool<T> * m_Pool;
		std::size_t m_Index;
		unsigned m_Version;

	public:

		Handle();
		Handle(Pool<T> & pool, std::size_t index);

		void reset();

		T & get();
		T & operator*();
		T * operator->();
		bool operator==(const Handle other) const;

		bool isValid() const;
	};



	template <typename T>
	Pool<T>::Handle::Handle()
	{
		reset();
	}

	template <typename T>
	Pool<T>::Handle::Handle(Pool<T> & pool, std::size_t index)
		: m_Pool(&pool)
		, m_Index(index)
		, m_Version(pool.version(index))
	{
	}

	template <typename T>
	void Pool<T>::Handle::reset()
	{
		m_Pool = nullptr;
		m_Index = std::numeric_limits<std::size_t>::max();
		m_Version = std::numeric_limits<unsigned>::max();
	}

	template <typename T>
	T & Pool<T>::Handle::get()
	{
		AlvAssert(isValid(), "Must check Pool<T>::Handle validity before getting a potentially invalid object");

		return m_Pool->get(m_Index);
	}

	template <typename T>
	T & Pool<T>::Handle::operator*()
	{
		AlvAssert(isValid(), "Must check Pool<T>::Handle validity before getting a potentially invalid object");

		return m_Pool->get(m_Index);
	}

	template <typename T>
	T * Pool<T>::Handle::operator->()
	{
		AlvAssert(isValid(), "Must check Pool<T>::Handle validity before getting a potentially invalid object");

		return &m_Pool->get(m_Index);
	}
	
	template <typename T>
	bool Pool<T>::Handle::operator==(const Pool<T>::Handle other) const
	{
		return other.m_Index == m_Index
			&& other.m_Pool == m_Pool
			&& other.m_Version == m_Version;
	}

	template <typename T>
	bool Pool<T>::Handle::isValid() const
	{
		return m_Index != std::numeric_limits<std::size_t>::max() && m_Pool->version(m_Index) == m_Version;
	}
}