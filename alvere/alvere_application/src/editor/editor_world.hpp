#pragma once

#include <string>

#include <alvere/graphics/camera.hpp>
#include <alvere\world\world.hpp>

namespace alvere
{
	class Window;
}

struct EditorWorld
{
	~EditorWorld()
	{
		int i = 0xBEEF;
	}

	std::string m_name;
	alvere::World m_world;
	alvere::Camera * m_camera;

	static std::unique_ptr<EditorWorld> New(const std::string & name, const alvere::Window & window);
};