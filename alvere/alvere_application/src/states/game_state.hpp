#pragma once

class GameState
{
public:

	virtual ~GameState() = default;

	virtual GameState * Update(float deltaTime) = 0;
	virtual void Render() = 0;
};