#pragma once

class GameState;

class GameStateMachine
{
	GameState * m_currentState;

public:

	GameStateMachine(GameState & initialState);

	void Update(float deltaTime);
	void Render();
};