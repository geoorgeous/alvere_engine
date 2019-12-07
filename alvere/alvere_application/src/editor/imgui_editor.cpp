#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <alvere/application/window.hpp>
#include <alvere/world/archetype/archetype_query.hpp>
#include <platform/windows/windows_window.hpp>
#include <alvere\world\component\components\c_camera.hpp>
#include <alvere\world\component\components\c_transform.hpp>

#include "imgui_editor.hpp"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "dialogs/open_file_dialog.hpp"
#include "editor/imgui_demo_window.hpp"
#include "editor/tile_window.hpp"
#include "editor/tile_properties_window.hpp"

ImGuiEditor::ImGuiEditor(alvere::Window & window)
	: m_window(window)
	, m_leftMouse(alvere::MouseButton::Left)
	, m_focusedMap(0)
{
	alvere::platform::windows::Window & castedWindow = (alvere::platform::windows::Window &)window;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiStyle & style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;

	ImGuiIO & io = ImGui::GetIO(); (void) io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplGlfw_InitForOpenGL(castedWindow.m_windowHandle, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	TileWindow & tileWindow = AddWindow<TileWindow>();
	AddWindow<TilePropertiesWindow>(tileWindow);
	AddWindow<ImGui_DemoWindow>();

	m_openMaps.push_back(EditorWorld::New("Castle", window));
	m_openMaps.push_back(EditorWorld::New("Not a castle", window));
}

ImGuiEditor::~ImGuiEditor()
{
	ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplGlfw_Shutdown(); //causes several nullrefs as the window is currently destroyed before this is called
	ImGui::DestroyContext();
}

void ImGuiEditor::Update(float deltaTime)
{
	m_leftMouse.Update(m_window);
	if (m_leftMouse.IsDown())
	{
		//Eventually we will want to abstract this into a pan tool implementation
		alvere::Archetype::Query cameraQuery;
		cameraQuery.Include<alvere::C_Transform>();
		cameraQuery.Include<alvere::C_Camera>();

		std::vector<std::reference_wrapper<alvere::Archetype>> cameras;
		m_focusedMap->m_world.QueryArchetypes(cameraQuery, cameras);

		alvere::C_Transform & cameraTransform = *cameras[0].get().GetProvider<alvere::C_Transform>().begin();
		cameraTransform->move({ 1, 1, 0 });
	}

	if (m_focusedMap != nullptr)
	{
		m_focusedMap->m_world.Update(deltaTime);
	}
}

void ImGuiEditor::Render()
{
	StartFrame();

	//Draw all the auxilliary windows first as using the ImGuiWindowFlags_NoBringToFrontOnFocus flag on the 
	//main window will put it at the back of the draw order, behind any windows using that flag in this list.
	for (std::unique_ptr<ImGui_Window> & window : m_windows)
	{
		if (window->m_visible)
		{
			window->Draw();
		}
	}

	//Always resize the main window to fill the whole background of the given window
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2((float) m_window.getWidth(), (float) 47.0f), ImGuiCond_Always);

	if (!ImGui::Begin("Main Window", NULL, m_windowflags))
	{
		ImGui::End();
		return;
	}

	DrawMenuBar();

	DrawMapTabs();

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

			const char * name = m_openMaps[i].m_name.c_str();

			if (ImGui::BeginTabItem(name, &active))
			{
				m_focusedMap = &m_openMaps[i];

				m_openMaps[i].m_world.Render();

				ImGui::EndTabItem();
			}

			if (active == false)
			{
				toRemove.push_back(i);
			}
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

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New", NULL, false, false))
		{
			std::cout << "Copied stuff" << std::endl;
		}

		if (ImGui::MenuItem("Open", NULL, false, true))
		{
			alvere::OpenFileDialog openFileDialog("Select a map to open", "", { "*.map" }, false);
			auto result = openFileDialog.Show();

			std::cout << result.first << std::endl;
			if (result.first)
				std::cout << result.second[0] << std::endl;
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::MenuItem("Copy", NULL, false, false))
		{
			std::cout << "Copied stuff" << std::endl;
		}

		if (ImGui::MenuItem("Paste", NULL, false, false))
		{
			std::cout << "Pasted stuff" << std::endl;
		}

		ImGui::EndMenu();
	}

	//Show all the main windows so they can be hidden/shown
	if (ImGui::BeginMenu("View"))
	{
		for (std::unique_ptr<ImGui_Window> & window : m_windows)
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

	ImGui::EndMenuBar();
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