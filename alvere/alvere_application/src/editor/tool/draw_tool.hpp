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

class DrawTool : public EditorTool
{
	TileWindow & m_tileWindow;
	alvere::Window & m_window;
	alvere::Camera & m_camera;
	C_Tilemap & m_tilemap;

	alvere::input::MouseButton m_leftMouse;

	int m_drawSize;

public:

	DrawTool(TileWindow & tileWindow, alvere::Window & window, alvere::Camera & camera, C_Tilemap & tilemap);

	void Update(float deltaTime) override;

private:

	void UpdateDraw();
	void UpdateDrawSize();
};