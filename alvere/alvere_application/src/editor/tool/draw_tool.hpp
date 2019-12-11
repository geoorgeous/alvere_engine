#pragma once

#include <alvere/input/mouse_button.hpp>

#include "editor_tool.hpp"

namespace alvere
{
	class Window;
	class Camera;
}

class ImGuiEditor;
class TileWindow;
class C_Tilemap;
class CommandStack;
class DrawTilesCommand;

class DrawTool : public EditorTool
{
	ImGuiEditor & m_editor;
	CommandStack & m_commandStack;
	TileWindow & m_tileWindow;
	alvere::Window & m_window;

	alvere::input::MouseButton m_leftMouse;

	int m_drawSize;

	DrawTilesCommand * m_activeDrawCommand;

public:

	DrawTool(ImGuiEditor & editor);

	void Update(float deltaTime) override;

private:

	void UpdateDraw();
	void UpdateDrawSize();
};