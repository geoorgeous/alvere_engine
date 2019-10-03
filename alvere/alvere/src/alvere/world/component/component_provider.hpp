#pragma once

#include <memory>

#include "alvere/world/component/component.hpp"

namespace alvere
{
	class ComponentProvider
	{
	public:

		virtual ~ComponentProvider() {}
		virtual ComponentProvider * CloneNew() = 0;

		virtual void Allocate() = 0;
		virtual void Deallocate(int entityIndex) = 0;
		virtual Component & GetComponent(int entityIndex) = 0;

		virtual void MoveEntityProvider(int entityIndex, ComponentProvider & other) = 0;
	};
}