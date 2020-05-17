#pragma once

#include <string>

#include <alvere/graphics/camera.hpp>
#include <alvere\world\world.hpp>

#include "components/tilemap/c_tilemap.hpp"

namespace alvere
{
	class Window;
}

struct EditorWorld
{
	bool m_dirty = false;
	bool m_requestClose = false;
	bool m_forceClose = false;

	std::string m_filepath;
	alvere::World m_world;
	alvere::Camera * m_camera;
	C_Tilemap * m_tilemap;

	std::string GetName() const;

	static std::unique_ptr<EditorWorld> New(const std::string & filepath, const alvere::Window & window);
};