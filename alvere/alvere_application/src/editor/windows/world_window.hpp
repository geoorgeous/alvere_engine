#pragma once

#include <imgui/imgui.h>
#include <alvere/input/key_button.hpp>
#include <alvere/world/entity/entity_handle.hpp>

#include "editor/command/command_stack.hpp"
#include "editor/windows/imgui_window.hpp"

namespace alvere
{
	class Window;
	class World;
	class Archetype;
}

class ImGuiEditor;

class WorldWindow : public ImGui_Window
{
	const ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_AlwaysVerticalScrollbar;

	ImGuiEditor & m_Editor;

	bool m_HideEmptyArchetypes = true;
	char m_Query[50];

public:

	WorldWindow(ImGuiEditor & editor);

	void Update(float deltaTime) override;

	void Draw() override;

	void DrawWorld(alvere::World & world);
	void DrawArchetype(alvere::Archetype & archetype);
	void DrawEntity(const alvere::EntityHandle & entity);

	virtual std::string GetName() const
	{
		return "World";
	}

};