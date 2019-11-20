#pragma once

#include <limits>

#include "alvere/utils/exceptions.hpp"
#include "alvere/utils/pool.hpp"

namespace alvere
{
	template <typename T>
	class Pool<T>::Handle
	{
		friend class Pool<T>;

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
		const T * operator->() const;
		bool operator==(const Handle other) const;

		bool isValid() const;

		struct Hash
		{
			size_t operator()(const typename Pool<T>::Handle & k) const
			{
				size_t hash = 17;
				hash = hash * 31 + std::hash<Pool<T>*>()(k.m_Pool);
				hash = hash * 31 + std::hash<std::size_t>()(k.m_Index);
				hash = hash * 31 + std::hash<unsigned>()(k.m_Version);
				return hash;
			}
		};
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
	const T * Pool<T>::Handle::operator->() const
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
		if (m_Index == std::numeric_limits<std::size_t>::max())
		{
			return false;
		}

		int pooledVersion = m_Pool->version(m_Index);
		return pooledVersion == m_Version;
	}
}