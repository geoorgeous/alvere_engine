#pragma once

#include "alvere/world/component/component.hpp"

namespace alvere
{
	template <typename T>
	class PooledComponent : public Component
	{
	public:
		class Provider;

		virtual ~PooledComponent() {};
	};
}

#include "alvere/world/component/pooled_component_provider.hpp"