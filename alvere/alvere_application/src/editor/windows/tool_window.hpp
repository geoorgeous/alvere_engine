#pragma once

#include <memory>

#include "imgui/imgui.h"
#include "editor/windows/imgui_window.hpp"
#include "editor\tool\editor_tool.hpp"

namespace alvere
{
	class Window;
}

class ImGuiEditor;

class ToolWindow : public ImGui_Window
{
	const ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_AlwaysAutoResize;
	const ImVec2 m_toolButtonSize = { 32, 32 };

	alvere::Window & m_window;

	ImGuiEditor & m_editor;

public:

	ToolWindow(ImGuiEditor & editor, alvere::Window & window);

	void Update(float deltaTime) override;

	void Draw() override;

	virtual std::string GetName() const
	{
		return "Tools";
	}

	EditorTool & GetCurrentTool() const
	{
		return *m_currentTool;
	}

private:

	std::unique_ptr<EditorTool> m_currentTool;
};