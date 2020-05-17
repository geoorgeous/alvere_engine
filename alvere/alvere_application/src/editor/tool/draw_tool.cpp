#include <alvere/application/window.hpp>
#include <alvere/graphics/camera.hpp>

#include "imgui/imgui.h"
#include "components/tilemap/c_tilemap.hpp"
#include "editor/editor_world.hpp"
#include "editor/imgui_editor.hpp"
#include "editor/command/command_stack.hpp"
#include "editor/command/draw_tiles_command.h"
#include "editor/windows/tile_window.hpp"
#include "editor/windows/history_window.hpp"
#include "editor/tool/draw_tool.hpp"

DrawTool::DrawTool(ImGuiEditor & editor)
	: m_editor(editor)
	, m_commandStack(editor.GetEditorWindow<HistoryWindow>()->m_commandStack)
	, m_tileWindow(*editor.GetEditorWindow<TileWindow>())
	, m_window(editor.GetApplicationWindow())
	, m_leftMouse(editor.GetApplicationWindow(), alvere::MouseButton::Left)
	, m_drawSize(0)
	, m_activeDrawCommand(nullptr)
{
}

void DrawTool::Update(float deltaTime)
{
	UpdateDrawSize();

	m_leftMouse.update();
	if (ImGui::GetIO().WantCaptureMouse == false && m_leftMouse.isDown())
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
	EditorWorld * world = m_editor.GetFocusedWorld();
	if (world == nullptr)
	{
		return;
	}

	alvere::Camera & camera = *world->m_camera;
	C_Tilemap & tilemap = *world->m_tilemap;

	EditorTile * selectedTile = m_tileWindow.GetSelectedTile();
	if (selectedTile == nullptr)
	{
		return;
	}
	
	alvere::Vector2 screenPosition = m_window.getMouse().position;
	alvere::Vector3 worldPosition = camera.screenToWorld(screenPosition, m_window.getSize().x, m_window.getSize().y);
	alvere::Vector2i tilePosition = tilemap.WorldToTilemap(worldPosition);

	alvere::Vector2i unit{ 1, 1 };
	alvere::Vector2i drawSize = { m_drawSize, m_drawSize };

	tilePosition -= drawSize;
	drawSize = drawSize * 2 + unit;

	alvere::RectI drawRect = { tilePosition, drawSize };

	if (drawRect.intersects(tilemap.GetBounds()) == false)
	{
		return;
	}

	if (m_activeDrawCommand == nullptr)
	{
		TileInstance toDraw{ &selectedTile->m_tile, { 0, 0 } };

		m_activeDrawCommand = new DrawTilesCommand(*world, toDraw);
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