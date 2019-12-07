#pragma once

#include <unordered_map>

#include "imgui/imgui.h"
#include "imgui_window.hpp"
#include "tilemap/tile_store.hpp"

class TileWindow : public ImGui_Window
{
	struct PositionHash
	{
		size_t operator()(const alvere::Vector2i & position) const noexcept
		{
			//This can be made much better, I doubt it will ever need to be however
			return position[0] * 31 ^ position[1] * 7;
		}
	};

	const ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_NoScrollbar;
	const ImVec2 m_tileSize = { 32, 32 };

	TileStore m_tiles;

	alvere::Vector2i m_selectedPosition;
	std::unordered_map<alvere::Vector2i, int, PositionHash> m_tilePositionMapping;

	std::unique_ptr<alvere::Texture> m_TEMP_tileTextureCollisionOn;
	std::unique_ptr<alvere::Texture> m_TEMP_tileTextureCollisionOff;

public:

	TileWindow();

	virtual void Draw() override;

	virtual std::string GetName() const override
	{
		return "Tile Palette";
	}

	Tile * GetSelectedTile() const
	{
		auto iter = m_tilePositionMapping.find(m_selectedPosition);
		return iter != m_tilePositionMapping.end()
			? m_tiles.GetTile(iter->second)
			: nullptr;
	}

private:

	void DrawTile(alvere::Vector2i position, alvere::Vector2i gridSize);
};