#pragma once

#include <alvere/application/window.hpp>

class ImGuiEditor
{
	alvere::Window & m_window;

public:

	ImGuiEditor(alvere::Window & window);
	~ImGuiEditor();

	void Render();

private:

	void StartFrame();
	void EndFrame();
};