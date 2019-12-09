#pragma once

class EditorTool
{
public:

	virtual ~EditorTool() = default;

	virtual void Update(float deltaTime) { };

	virtual void Render() { };
};