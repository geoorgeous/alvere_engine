#pragma once

#include "alvere/world/component/tag_component.hpp"
#include "alvere/world/component/component_provider.hpp"

template <typename T>
class TagComponent<T>::Provider : public ComponentProvider
{
	static Component* s_ComponentInstance;

public:
	class iterator;

	virtual ComponentProvider* CloneNew() override
	{
		return new TagComponent<T>::Provider();
	}

	virtual void Allocate() override
	{
	}

	virtual void Deallocate( int entityIndex ) override
	{
	}

	virtual void MoveEntityProvider( int entityIndex, ComponentProvider& other ) override
	{
	}

	virtual Component& GetComponent( int entityIndex ) override
	{
		return *s_ComponentInstance;
	}

	iterator begin();
};

template <typename T>
Component* TagComponent<T>::Provider::s_ComponentInstance = new TagComponent<T>();

#include "alvere/world/component/tag_component_provider_iterator.hpp"

template <typename T>
typename TagComponent<T>::Provider::iterator TagComponent<T>::Provider::begin()
{
	return TagComponent<T>::Provider::iterator( this );
}