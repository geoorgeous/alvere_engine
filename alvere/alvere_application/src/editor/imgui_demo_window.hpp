#pragma once

#include "imgui/imgui.h"
#include "windows/imgui_window.hpp"

class ImGui_DemoWindow : public ImGui_Window
{
public:

	virtual void Draw() override
	{
		ImGui::ShowDemoWindow(&m_visible);
	}

	virtual std::string GetName() const override
	{
		return "ImGui_DemoWindow";
	}
};
