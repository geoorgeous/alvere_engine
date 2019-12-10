#include <alvere/alvere.hpp>

#include "states/game_state_machine.hpp"
#include "states/gameplay_state.hpp"
#include "states/editor_state.hpp"

using namespace alvere;

struct AlvereApplication : public Application
{
	GameStateMachine m_stateMachine;

	AlvereApplication()
		: Application(Window::Properties("Platformer", Vec2i{ 1024, 768 }))
		, m_stateMachine(*new GameplayState(*m_window))
		//, m_stateMachine(*new EditorState(*m_window))
	{
	}

	void update(float deltaTime) override
	{
		m_stateMachine.Update(deltaTime);
	}

	void render() override
	{
		m_stateMachine.Render();
	}
};

ALV_NEW_APP_FUNC(AlvereApplication)