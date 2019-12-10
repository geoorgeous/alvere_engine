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
	m_undoButton.Update();
	if (m_undoButton.IsPressed())
	{
		m_commandStack.Undo();
	}

	m_redoButton.Update();
	if (m_redoButton.IsPressed())
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

	//Draw inactive history elements first
	for (int i = 0; i < m_commandStack.m_currentOffset; ++i)
	{
		ImGui::PushID(i);

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(0.05f, 0.10f, 0.16f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor(0.10f, 0.20f, 0.32f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor(0.20f, 0.39f, 0.62f));
		
		DrawHistoryElement(*commandHistory[numCommands - i - 1]);

		ImGui::PopStyleColor(3);

		ImGui::PopID();
	}

	//Draw active history elements next
	for (int i = m_commandStack.m_currentOffset; i < numCommands; ++i)
	{
		ImGui::PushID(i);

		DrawHistoryElement(*commandHistory[numCommands - i - 1]);

		ImGui::PopID();
	}

	ImGui::PopItemWidth();
	

	ImGui::End();
}

void HistoryWindow::DrawHistoryElement(Command & command)
{
	ImGui::Button(command.GetDescription().c_str(), { ImGui::CalcItemWidth(), 0 });
}