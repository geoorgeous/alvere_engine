#include "editor_state.hpp"
#include "states/gameplay_state.hpp"

EditorState::EditorState(alvere::Window & window)
	: m_window(window)
	, m_toggleEditor(window, alvere::Key::I)
	, m_editor(window)
{
}

GameState * EditorState::Update(float deltaTime)
{
	m_toggleEditor.update();
	if (m_toggleEditor.isReleased())
	{
		return new GameplayState(m_window);
	}

	m_editor.Update(deltaTime);

	return nullptr;
}

void EditorState::Render()
{
	m_editor.Render();
}