#pragma once

#include "alvere/world/component/tag_component_provider.hpp"

namespace alvere
{
	template <typename T>
	class TagComponent<T>::Provider::iterator
	{
		TagComponent<T>::Provider * m_Provider;

	public:

		iterator()
			: m_Provider(nullptr)
		{
		}

		iterator(TagComponent<T>::Provider * provider)
			: m_Provider(provider)
		{
		}

		iterator & operator++()
		{
			return *this;
		}

		iterator operator++(int)
		{
			iterator retval = *this;
			++(*this);
			return retval;
		}

		bool operator==(iterator other) const
		{
			return m_Provider == other.m_Provider;
		}

		bool operator!=(iterator other) const
		{
			return !(*this == other);
		}

		T & operator*()
		{
			return static_cast<T &>(m_Provider->GetComponent(0));
		}

		T * operator->()
		{
			return static_cast<T *>(&m_Provider->GetComponent(0));
		}

		// iterator traits
		using difference_type = std::size_t;
		using value_type = T;
		using pointer = T *;
		using reference = T &;
		using iterator_category = std::forward_iterator_tag;
	};
}