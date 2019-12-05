
#include "imgui_editor.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#include <alvere/application/window.hpp>
#include <platform/windows/windows_window.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>



ImGuiEditor::ImGuiEditor(alvere::Window & window)
	: m_window(window)
{
	alvere::platform::windows::Window & castedWindow = (alvere::platform::windows::Window &)m_window;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO & io = ImGui::GetIO(); (void) io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplGlfw_InitForOpenGL(castedWindow.m_windowHandle, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

ImGuiEditor::~ImGuiEditor()
{
	ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplGlfw_Shutdown(); //causes several nullrefs as the window is currently destroyed before this is called
	ImGui::DestroyContext();
}

void ImGuiEditor::Render()
{
	StartFrame();

	ImGui::ShowDemoWindow();

	EndFrame();
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