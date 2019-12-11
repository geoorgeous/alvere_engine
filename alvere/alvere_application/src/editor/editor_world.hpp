#pragma once

#include <string>

#include <alvere/graphics/camera.hpp>
#include <alvere\world\world.hpp>

#include "tilemap/c_tilemap.hpp"

namespace alvere
{
	class Window;
}

struct EditorWorld
{
	std::string m_filepath;
	alvere::World m_world;
	alvere::Camera * m_camera;
	C_Tilemap * m_tilemap;

	static std::unique_ptr<EditorWorld> New(const std::string & filepath, const alvere::Window & window);
};