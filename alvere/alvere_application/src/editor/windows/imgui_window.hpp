#pragma once

#include <string>

class ImGui_Window
{
public:

	bool m_visible = true;

	virtual void Update(float deltaTime) {};

	virtual void Draw() = 0;

	virtual std::string GetName() const = 0;

	virtual bool AddToViewMenu() const { return true; }
};