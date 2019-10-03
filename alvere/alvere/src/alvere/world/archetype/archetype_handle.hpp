#pragma once

#include <vector>
#include <typeindex>

#include "Assert.h"
#include "alvere/world/archetype/archetype.hpp"

class Archetype::Handle
{
	std::vector<std::type_index> m_Types;
	std::size_t m_Hash;

public:
	friend struct std::hash<Archetype::Handle>;
	
	Handle()
		: m_Hash( 0 )
	{
	}

	bool operator==( const Handle& other ) const
	{
		//TODO: Should probs also check through the type vector here as otherwise two sets of component
		//		types could potentially hash to the same value even though they have different values.
		return m_Hash == other.m_Hash;
	}

	template <typename T>
	void AddComponent();
	void AddComponent( const std::type_index& type );

	template <typename T>
	void RemoveComponent();
	void RemoveComponent( const std::type_index& type );

	template <typename... Components>
	static Archetype::Handle make_handle();
};

template <typename T>
void Archetype::Handle::AddComponent()
{
	AddComponent( typeid( T ) );
}

template <typename T>
void Archetype::Handle::RemoveComponent()
{
	RemoveComponent( typeid( T ) );
}

template <typename... Components>
static Archetype::Handle Archetype::Handle::make_handle()
{
	Archetype::Handle handle;
	( handle.AddComponent<Components>(), ... );
	return handle;
}

namespace std
{
	template <>
	struct hash<Archetype::Handle>
	{
		std::size_t operator()( const Archetype::Handle& k ) const
		{
			return k.m_Hash;
		}
	};
}