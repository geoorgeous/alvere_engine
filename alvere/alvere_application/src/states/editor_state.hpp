#pragma once

#include <alvere/input/key_button.hpp>

#include "game_state.hpp"
#include "editor/imgui_editor.hpp"

namespace alvere
{
	class Window;
}

class EditorState : public GameState
{
	alvere::Window & m_window;

	alvere::input::KeyButton m_toggleEditor;

	ImGuiEditor m_editor;

public:

	EditorState(alvere::Window & window);

	GameState * Update(float deltaTime) override;
	void Render() override;
};