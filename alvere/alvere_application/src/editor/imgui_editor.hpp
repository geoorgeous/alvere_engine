#pragma once

#include "imgui/imgui.h"
#include "windows/imgui_window.hpp"
#include "editor_world.hpp"
#include "editor/windows/tile_window.hpp"
#include "editor/windows/tile_properties_window.hpp"
#include "editor/io/world_exporter.hpp"
#include "editor/io/world_importer.hpp"

namespace alvere
{
	class Window;
}

class ImGuiEditor
{
	enum class ModalPopupState
	{
		None,
		ResizeTilemap,
		UnsavedChanges,
	};

	const ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_MenuBar
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoScrollWithMouse;

	alvere::Window & m_window;

	std::vector<std::unique_ptr<ImGui_Window>> m_editorWindows;

	std::vector<std::unique_ptr<EditorWorld>> m_openMaps;
	EditorWorld * m_focusedMap;

	ModalPopupState m_currentPopup;

	WorldImporter m_importer;
	WorldExporter m_exporter;

public:

	ImGuiEditor(alvere::Window & window);
	~ImGuiEditor();

	void Update(float deltaTime);
	void Render();

	alvere::Window & GetApplicationWindow() const;

	EditorWorld * GetFocusedWorld() const;

	template <typename T>
	T * GetEditorWindow();

private:

	void StartFrame();
	void EndFrame();

	void DrawMenuBar();
	void DrawFileMenu();
	void DrawEditMenu();
	void DrawViewMenu();

	void DrawMapTabs();

	void OpenPopup(ModalPopupState popup);
	void DrawPopups();
	void DrawResizePopup();
	void DrawUnsavedChangesPopup();

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

	m_editorWindows.emplace_back(std::move(window));

	return static_cast<T&>(*m_editorWindows.back());
}

template <typename T>
T * ImGuiEditor::GetEditorWindow()
{
	for (auto & window : m_editorWindows)
	{
		T * casted = dynamic_cast<T *>(window.get());
		if (casted != nullptr)
		{
			return casted;
		}
	}

	return nullptr;
}