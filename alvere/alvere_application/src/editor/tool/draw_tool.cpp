#include <alvere/application/window.hpp>
#include <alvere/graphics/camera.hpp>

#include "draw_tool.hpp"
#include "editor/windows/tile_window.hpp"
#include "tilemap/c_tilemap.hpp"
#include "editor/command/command_stack.hpp"
#include "editor/command/draw_tiles_command.h"

DrawTool::DrawTool(CommandStack & commandStack, TileWindow & tileWindow, alvere::Window & window, alvere::Camera & camera, C_Tilemap & tilemap)
	: m_commandStack(commandStack)
	, m_tileWindow(tileWindow)
	, m_window(window)
	, m_camera(camera)
	, m_tilemap(tilemap)
	, m_leftMouse(window, alvere::MouseButton::Left)
	, m_drawSize(0)
{
}

void DrawTool::Update(float deltaTime)
{
	UpdateDrawSize();

	m_leftMouse.Update();
	if (m_leftMouse.IsPressed())
	{
		UpdateDraw();
	}
}

void DrawTool::UpdateDraw()
{
	EditorTile * selectedTile = m_tileWindow.GetSelectedTile();
	if (selectedTile == nullptr)
	{
		return;
	}
	
	alvere::Vector2 screenPosition = m_window.getMouse().position;
	alvere::Vector3 worldPosition = m_camera.screenToWorld(screenPosition, m_window.getWidth(), m_window.getHeight());
	alvere::Vector2i tilePosition = m_tilemap.WorldToTilemap(worldPosition);

	alvere::Vector2i unit{ 1, 1 };
	alvere::Vector2i drawSize = { m_drawSize, m_drawSize };

	tilePosition -= drawSize;
	drawSize = drawSize * 2 + unit;

	alvere::RectI drawRect = { tilePosition, drawSize };

	if (drawRect.intersects(m_tilemap.GetBounds()) == false)
	{
		return;
	}

	m_commandStack.Add(new DrawTilesCommand(m_tilemap, drawRect, &selectedTile->m_tile));
}

void DrawTool::UpdateDrawSize()
{
	float scroll = m_window.getMouse().scrollDelta.y;
	if (scroll > 0)
	{
		m_drawSize += 1;
	}
	else if (scroll < 0 && m_drawSize > 0)
	{
		m_drawSize -= 1;
	}
}