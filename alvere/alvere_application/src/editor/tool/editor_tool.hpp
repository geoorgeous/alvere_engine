#pragma once

class EditorTool
{
public:

	virtual void Update(float deltaTime) = 0;

	virtual void Render() = 0;
};