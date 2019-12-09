#include <alvere/application/window.hpp>
#include <alvere/graphics/camera.hpp>

#include "draw_tool.hpp"
#include "editor/windows/tile_window.hpp"
#include "tilemap/c_tilemap.hpp"

DrawTool::DrawTool(TileWindow & tileWindow, alvere::Window & window, alvere::Camera & camera, C_Tilemap & tilemap)
	: m_tileWindow(tileWindow)
	, m_window(window)
	, m_camera(camera)
	, m_tilemap(tilemap)
	, m_leftMouse(window, alvere::MouseButton::Left)
{
}

void DrawTool::Update(float deltaTime)
{
	m_leftMouse.Update();

	if (m_leftMouse.IsDown() == false)
	{
		return;
	}

	EditorTile * selectedTile = m_tileWindow.GetSelectedTile();
	if (selectedTile == nullptr)
	{
		return;
	}

	alvere::Vector2 mouseScreenPosition = m_window.getMouse().position;
	alvere::Vector3 mouseWorldPosition = m_camera.screenToWorld(mouseScreenPosition, m_window.getWidth(), m_window.getHeight());
	alvere::Vector2i mouseTilePosition = m_tilemap.WorldToTilemap(mouseWorldPosition);
	m_tilemap.SetTile(mouseTilePosition, &selectedTile->m_tile);
}