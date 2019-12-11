#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <alvere/application/window.hpp>
#include <alvere/math/matrix/transformations.hpp>
#include <alvere/world/archetype/archetype_query.hpp>
#include <platform/windows/windows_window.hpp>
#include <alvere\world\component\components\c_camera.hpp>
#include <alvere\world\component\components\c_transform.hpp>
#include <alvere/debug/logging.hpp>

#include "imgui_editor.hpp"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "dialogs/open_file_dialog.hpp"
#include "dialogs/save_file_dialog.hpp"
#include "editor/imgui_demo_window.hpp"
#include "editor/windows/tool_window.hpp"
#include "editor/windows/history_window.hpp"
#include "editor\tool\pan_tool.hpp"
#include "editor/utils/path_utils.hpp"

ImGuiEditor::ImGuiEditor(alvere::Window & window)
	: m_window(window)
	, m_focusedMap(0)
	, m_importer(*this, window)
{
	alvere::platform::windows::Window & castedWindow = (alvere::platform::windows::Window &)window;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiStyle & style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;

	ImGuiIO & io = ImGui::GetIO(); (void) io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	ImGui_ImplGlfw_InitForOpenGL(castedWindow.getHandle(), false);
	ImGui_ImplOpenGL3_Init("#version 130");

	TileWindow & tileWindow = AddWindow<TileWindow>();
	AddWindow<TilePropertiesWindow>(tileWindow);
	AddWindow<ToolWindow>(*this, window);
	AddWindow<HistoryWindow>(window);
	AddWindow<ImGui_DemoWindow>();

	m_openMaps.push_back(EditorWorld::New("res/maps/demo.map", m_window));
}

ImGuiEditor::~ImGuiEditor()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiEditor::Update(float deltaTime)
{
	//Draw all the auxilliary windows first as using the ImGuiWindowFlags_NoBringToFrontOnFocus flag on the 
	//main window will put it at the back of the draw order, behind any windows using that flag in this list.
	for (std::unique_ptr<ImGui_Window> & window : m_editorWindows)
	{
		if (window->m_visible)
		{
			window->Update(deltaTime);
		}
	}

	if (m_focusedMap == nullptr)
	{
		return;
	}

	m_focusedMap->m_world.Update(deltaTime);
}

void ImGuiEditor::Render()
{
	StartFrame();

	//Draw all the auxilliary windows first as using the ImGuiWindowFlags_NoBringToFrontOnFocus flag on the 
	//main window will put it at the back of the draw order, behind any windows using that flag in this list.
	for (std::unique_ptr<ImGui_Window> & window : m_editorWindows)
	{
		if (window->m_visible)
		{
			window->Draw();
		}
	}

	//Always resize the main window to fill the whole background of the given window
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2((float) m_window.getRenderingContext().getResolution().x, (float) 47.0f), ImGuiCond_Always);

	if (!ImGui::Begin("Main Window", NULL, m_windowflags))
	{
		ImGui::End();
		return;
	}

	m_currentPopup = ModalPopupState::None;

	DrawMenuBar();

	DrawMapTabs();

	if (m_currentPopup != ModalPopupState::None)
	{
		OpenPopup(m_currentPopup);
	}

	DrawResizePopup();

	ImGui::End();

	EndFrame();
}


void ImGuiEditor::DrawMapTabs()
{
	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable
		| ImGuiTabBarFlags_AutoSelectNewTabs
		| ImGuiTabBarFlags_FittingPolicyResizeDown;

	if (ImGui::BeginTabBar("Open Levels", tab_bar_flags))
	{
		m_focusedMap = nullptr;
		std::vector<int> toRemove;
		for (int i = 0; i < m_openMaps.size(); ++i)
		{
			bool active = true;

			std::string filename;
			GetFilenameFromPath(m_openMaps[i]->m_filepath, filename);
			const char * name = filename.c_str();
			ImGui::PushID(i);
			if (ImGui::BeginTabItem(name, &active))
			{
				m_focusedMap = m_openMaps[i].get();

				m_openMaps[i]->m_world.Render();

				ImGui::EndTabItem();
			}

			if (active == false)
			{
				toRemove.push_back(i);
			}
			ImGui::PopID();
		}

		//Remove any tabs that were closed
		for (int i = (int) toRemove.size() - 1; i >= 0; --i)
		{
			m_openMaps.erase(m_openMaps.begin() + toRemove[i]);
		}

		ImGui::EndTabBar();
	}
}

void ImGuiEditor::DrawMenuBar()
{
	if (ImGui::BeginMenuBar() == false)
	{
		return;
	}

	DrawFileMenu();

	DrawEditMenu();

	DrawViewMenu();

	ImGui::EndMenuBar();
}

void ImGuiEditor::DrawFileMenu()
{
	if (ImGui::BeginMenu("File") == false)
	{
		return;
	}

	if (ImGui::MenuItem("New", NULL, false, true))
	{
		alvere::SaveFileDialog newMapDialog("Create New Map", "", { "*.map" });

		std::pair<bool, std::string> newMapValue = newMapDialog.Show();

		if (newMapValue.first)
		{
			m_openMaps.push_back(EditorWorld::New(newMapValue.second, m_window));
		}
	}

	ImGui::Separator();

	if (ImGui::MenuItem("Open", NULL, false, true))
	{
		alvere::OpenFileDialog openFileDialog("Select a map to open", "", { "*.map" }, false);
		auto result = openFileDialog.Show();

		if (result.first && result.second.size() > 0)
		{
			std::string filepath = result.second[0];


			m_openMaps.push_back(m_importer(filepath));
		}
	}

	ImGui::Separator();

	std::string mapName = "";
	EditorWorld * world = GetFocusedWorld();
	if (world != nullptr)
	{
		GetFilenameFromPath(world->m_filepath, mapName);
	}

	std::string saveLabel = (world == nullptr ? "Save" : "Save " + mapName);
	if (ImGui::MenuItem(saveLabel.c_str(), NULL, false, world != nullptr) && world != nullptr)
	{
		m_exporter(world->m_filepath, *world);
	}

	std::string saveAsLabel = ( world == nullptr ? "Save As" : "Save " + mapName + " As" );
	if (ImGui::MenuItem(saveAsLabel.c_str(), NULL, false, world != nullptr) && world != nullptr)
	{
		std::wstring path = std::filesystem::absolute(world->m_filepath);
		alvere::SaveFileDialog newMapDialog("Save Map As", std::string(path.begin(), path.end()), { "*.map" });

		std::pair<bool, std::string> newMapValue = newMapDialog.Show();

		if (newMapValue.first)
		{
			std::string newFilepath = newMapValue.second;
			if (HasExtension(newFilepath) == false)
			{
				newFilepath += ".map";
			}

			//Store this new path back into the world
			world->m_filepath = newFilepath;

			m_exporter(newFilepath, *world);
		}
	}

	ImGui::EndMenu();
}

void ImGuiEditor::DrawEditMenu()
{
	if (ImGui::BeginMenu("Edit") == false)
	{
		return;
	}

	if (ImGui::MenuItem("Copy", NULL, false, false))
	{
		std::cout << "Copied stuff" << std::endl;
	}

	if (ImGui::MenuItem("Paste", NULL, false, false))
	{
		std::cout << "Pasted stuff" << std::endl;
	}

	ImGui::Separator();

	if (ImGui::MenuItem("Resize Tilemap", NULL, false))
	{
		m_currentPopup = ModalPopupState::ResizeTilemap;
	}

	ImGui::EndMenu();
}

void ImGuiEditor::DrawViewMenu()
{
	if (ImGui::BeginMenu("View") == false)
	{
		return;
	}

	//Draw all the registered windows so they can be hidden/shown by the user
	for (std::unique_ptr<ImGui_Window> & window : m_editorWindows)
	{
		if (window->AddToViewMenu() == false)
		{
			continue;
		}

		if (ImGui::MenuItem(window->GetName().c_str(), NULL, window->m_visible))
		{
			window->m_visible = !window->m_visible;
		}
	}

	ImGui::EndMenu();
}

void ImGuiEditor::OpenPopup(ModalPopupState state)
{
	switch (state)
	{
		case ModalPopupState::ResizeTilemap:
			ImGui::OpenPopup("Resize Tilemap");
			return;
	}
}

void ImGuiEditor::DrawPopups()
{
	DrawResizePopup();
}

void ImGuiEditor::DrawResizePopup()
{
	static alvere::Vector2i min{ 0, 0 };
	static alvere::Vector2i max{ 0, 0 };

	EditorWorld * world = GetFocusedWorld();
	if (world == nullptr)
	{
		return;
	}

	if (ImGui::BeginPopupModal("Resize Tilemap", NULL, ImGuiWindowFlags_AlwaysAutoResize) == false)
	{
		min = { 0, 0 };
		max = { 0, 0 };
		return;
	}

	ImGui::Text("Expand or contract the tilemap");

	ImGui::Separator();

	ImGui::InputInt("Left", &min[0]);
	ImGui::InputInt("Right", &max[0]);
	ImGui::InputInt("Up", &max[1]);
	ImGui::InputInt("Down", &min[1]);

	ImGui::Separator();

	if (ImGui::Button("OK", ImVec2(120, 0)))
	{
		if (world != nullptr)
		{
			world->m_tilemap->Resize(min[0], max[0], max[1], min[1]);
		}

		ImGui::CloseCurrentPopup();
	}

	ImGui::SameLine();

	if (ImGui::Button("Cancel", ImVec2(120, 0)))
	{
		ImGui::CloseCurrentPopup();
	}

	ImGui::EndPopup();
}



void ImGuiEditor::StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiEditor::EndFrame()
{
	ImGui::Render();

	alvere::platform::windows::Window & castedWindow = (alvere::platform::windows::Window &)m_window;

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

EditorWorld * ImGuiEditor::GetFocusedWorld() const
{
	return m_focusedMap;
}