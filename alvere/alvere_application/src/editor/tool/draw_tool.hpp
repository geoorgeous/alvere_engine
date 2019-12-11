#pragma once

#include <alvere/input/mouse_button.hpp>

#include "editor_tool.hpp"

namespace alvere
{
	class Window;
	class Camera;
}

class TileWindow;
class C_Tilemap;
class CommandStack;
class DrawTilesCommand;

class DrawTool : public EditorTool
{
	CommandStack & m_commandStack;
	TileWindow & m_tileWindow;
	alvere::Window & m_window;
	alvere::Camera & m_camera;
	C_Tilemap & m_tilemap;

	alvere::input::MouseButton m_leftMouse;

	int m_drawSize;

	DrawTilesCommand * m_activeDrawCommand;

public:

	DrawTool(CommandStack & commandStack, TileWindow & tileWindow, alvere::Window & window, alvere::Camera & camera, C_Tilemap & tilemap);

	void Update(float deltaTime) override;

private:

	void UpdateDraw();
	void UpdateDrawSize();
};