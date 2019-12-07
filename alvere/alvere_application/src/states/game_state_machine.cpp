#include "game_state_machine.hpp"
#include "game_state.hpp"

GameStateMachine::GameStateMachine(GameState & initialState)
	: m_currentState(&initialState)
{
}

void GameStateMachine::Update(float deltaTime)
{
	GameState * nextState = m_currentState->Update(deltaTime);

	if (nextState == nullptr)
	{
		return;
	}

	delete m_currentState;
	m_currentState = nextState;
}

void GameStateMachine::Render()
{
	m_currentState->Render();
}