#pragma once

#include <vector>
#include <cassert>

#include "alvere/world/component/component_provider.hpp"
#include "alvere/world/component/pooled_component.hpp"

template <typename T>
class PooledComponent<T>::Provider : public ComponentProvider
{
	std::vector<T> m_Pool;

public:
	using iterator = typename std::vector<T>::iterator;

	Provider() = default;
	Provider( const Provider& ) = delete;
	Provider& operator=( const Provider& ) = delete;
	virtual ComponentProvider* CloneNew() override;

	virtual void Allocate() override;
	virtual void Deallocate( int entityIndex ) override;
	virtual void MoveEntityProvider( int entityIndex, ComponentProvider& other ) override;

	virtual Component& GetComponent( int entityIndex ) override;

	iterator begin();
	iterator end();
};



template <typename T>
typename void PooledComponent<T>::Provider::Allocate()
{
	m_Pool.emplace_back();
}

template <typename T>
void PooledComponent<T>::Provider::Deallocate( int entityIndex )
{
	m_Pool[ entityIndex ] = m_Pool.back();
	m_Pool.pop_back();
}

template <typename T>
void PooledComponent<T>::Provider::MoveEntityProvider( int entityIndex, ComponentProvider& other )
{
	PooledComponent<T>::Provider& typedOther = static_cast<PooledComponent<T>::Provider&>( other );

	typedOther.Allocate();

	T& newComponent = typedOther.m_Pool.back();
	T& oldComponent = static_cast<T&>( GetComponent( entityIndex ) );

	newComponent = std::move( oldComponent );

	Deallocate( entityIndex );
}

template <typename T>
ComponentProvider* PooledComponent<T>::Provider::CloneNew()
{
	return new PooledComponent<T>::Provider();
}

template <typename T>
Component& PooledComponent<T>::Provider::GetComponent( int entityIndex )
{
	return m_Pool[ entityIndex ];
}

template <typename T>
typename PooledComponent<T>::Provider::iterator PooledComponent<T>::Provider::begin()
{
	return m_Pool.begin();
}

template <typename T>
typename PooledComponent<T>::Provider::iterator PooledComponent<T>::Provider::end()
{
	return m_Pool.end();
}