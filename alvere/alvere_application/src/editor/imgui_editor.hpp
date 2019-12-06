#pragma once

#include <alvere/application/window.hpp>

#include "imgui/imgui.h"
#include "imgui_window.hpp"

class ImGuiEditor
{
	const ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_MenuBar
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoScrollWithMouse;

	alvere::Window & m_window;

	std::vector<std::unique_ptr<ImGui_Window>> m_windows;

	std::vector<std::string> m_openLevels;

public:

	ImGuiEditor(alvere::Window & window);
	~ImGuiEditor();

	void Render();

private:

	void StartFrame();
	void EndFrame();

	void DrawMenuBar();
	void DrawBoardTabs();

	template <typename T, typename... Args>
	T & AddWindow(Args &&... args);

};


template <typename T, typename... Args>
T & ImGuiEditor::AddWindow(Args &&... args)
{
	std::unique_ptr<T> window = std::make_unique<T>(std::forward<Args>( args )...);

	if (window->AddToViewMenu() == false)
	{
		window->m_visible = true;
	}

	m_windows.emplace_back(std::move(window));

	return static_cast<T&>(*m_windows.back());
}