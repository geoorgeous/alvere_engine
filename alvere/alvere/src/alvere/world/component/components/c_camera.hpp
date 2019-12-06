#pragma once

#include "alvere/world/component/pooled_component.hpp"
#include "alvere/graphics/camera.hpp"

namespace alvere
{
	//Just a wrapper for the standalone camera class
	class C_Camera : public PooledComponent<C_Camera>, public Camera
	{
	};
}