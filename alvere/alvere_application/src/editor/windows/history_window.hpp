#pragma once

#include <alvere/input/key_button.hpp>

#include "editor/command/command_stack.hpp"
#include "editor/windows/imgui_window.hpp"

namespace alvere
{
	class Window;
}

class HistoryWindow : public ImGui_Window
{
	const ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_None;

	alvere::input::KeyButton m_undoButton;
	alvere::input::KeyButton m_redoButton;

public:

	CommandStack m_commandStack;


	HistoryWindow(alvere::Window & window);

	void Update(float deltaTime) override;

	void Draw() override;

	virtual std::string GetName() const
	{
		return "History";
	}

private:

	bool DrawHistoryElement(Command & command);
};