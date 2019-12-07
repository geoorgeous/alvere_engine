#pragma once

#include <alvere\input\mouse_button.hpp>

#include "editor_tool.hpp"

namespace alvere
{
	class Window;
}

class ImGuiEditor;

class PanTool : public EditorTool
{
	ImGuiEditor & m_editor;
	alvere::Window & m_window;

	alvere::input::MouseButton m_leftMouse;
	alvere::Vector2 m_mousePosition;

public:

	PanTool(ImGuiEditor & editor, alvere::Window & window);

	void Update(float deltaTime) override;

	void Render() override;
};