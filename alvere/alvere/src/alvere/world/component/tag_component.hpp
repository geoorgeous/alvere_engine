#pragma once

#include "alvere/world/component/component.hpp"

namespace alvere
{
	template <typename T>
	class TagComponent : public Component
	{
	public:
		class Provider;
	};
}

#include "alvere/world/component/tag_component_provider.hpp"