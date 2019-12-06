#pragma once

#include <string>

#include <alvere\world\world.hpp>

namespace alvere
{
	class Window;
}

struct EditorWorld
{
	std::string m_name;
	alvere::World m_world;

	static EditorWorld New(const std::string & name, const alvere::Window & window);
};