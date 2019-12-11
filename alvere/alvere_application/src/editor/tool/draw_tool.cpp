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
	, m_activeDrawCommand(nullptr)
{
}

void DrawTool::Update(float deltaTime)
{
	UpdateDrawSize();

	m_leftMouse.Update();
	if (m_leftMouse.IsDown())
	{
		UpdateDraw();
	}
	else
	{
		m_activeDrawCommand = nullptr;
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
	alvere::Vector3 worldPosition = m_camera.screenToWorld(screenPosition, m_window.getSize().x, m_window.getSize().y);
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

	if (m_activeDrawCommand == nullptr)
	{
		TileInstance toDraw{ &selectedTile->m_tile, { 0, 0 } };

		m_activeDrawCommand = new DrawTilesCommand(m_tilemap, toDraw);
		m_commandStack.Add(m_activeDrawCommand);
	}

	m_activeDrawCommand->AddDrawArea(drawRect);
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