#pragma once

#include "imgui/imgui.h"
#include "imgui_window.hpp"

#include "tile_window.hpp"

class TilePropertiesWindow : public ImGui_Window
{
	const ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_None;

	TileWindow & m_tileWindow;

public:

	TilePropertiesWindow(TileWindow & tileWindow);

	virtual void Draw() override;

	virtual std::string GetName() const override
	{
		return "Tile Properties";
	}

private:

	void UserSetTileTexture(EditorTile & tile);
};