#include <alvere/application/window.hpp>

#include "imgui/imgui.h"
#include "editor/windows/history_window.hpp"
#include "editor/command/command.hpp"

HistoryWindow::HistoryWindow(alvere::Window & window)
	: m_undoButton(window, alvere::Key::Z)
	, m_redoButton(window, alvere::Key::Y)
{
}

void HistoryWindow::Update(float deltaTime)
{
	m_undoButton.update();
	if (m_undoButton.isPressed())
	{
		m_commandStack.Undo();
	}

	m_redoButton.update();
	if (m_redoButton.isPressed())
	{
		m_commandStack.Redo();
	}
}

void HistoryWindow::Draw()
{
	ImGui::Begin(GetName().c_str(), &m_visible, m_windowflags);

	ImGuiStyle & style = ImGui::GetStyle();

	ImGui::PushItemWidth(-1);

	auto & commandHistory = m_commandStack.m_commandHistory;

	int numCommands = m_commandStack.m_commandHistory.size();

	int clickedIndex = -1;

	//Draw inactive history elements first
	for (int i = 0; i < m_commandStack.m_currentOffset; ++i)
	{
		ImGui::PushID(i);

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(0.05f, 0.10f, 0.16f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor(0.10f, 0.20f, 0.32f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor(0.20f, 0.39f, 0.62f));
		
		if (DrawHistoryElement(*commandHistory[numCommands - i - 1]))
		{
			clickedIndex = i;
		}

		ImGui::PopStyleColor(3);

		ImGui::PopID();
	}

	//Draw active history elements next
	for (int i = m_commandStack.m_currentOffset; i < numCommands; ++i)
	{
		ImGui::PushID(i);

		if (DrawHistoryElement(*commandHistory[numCommands - i - 1]))
		{
			clickedIndex = i;
		}

		ImGui::PopID();
	}

	ImGui::PopItemWidth();

	ImGui::End();

	//Check if the user has clicked one of the undo commands to navigate to
	if (clickedIndex == -1)
	{
		return;
	}

	int offset = m_commandStack.m_currentOffset;
	if (clickedIndex < offset)
	{
		for (int i = clickedIndex; i < offset; ++i)
		{
			m_commandStack.Redo();
		}
	}
	else if (clickedIndex > offset)
	{
		for (int i = offset; i < clickedIndex; ++i)
		{
			m_commandStack.Undo();
		}
	}
}

bool HistoryWindow::DrawHistoryElement(Command & command)
{
	return ImGui::Button(command.GetDescription().c_str(), { ImGui::CalcItemWidth(), 0 });
}