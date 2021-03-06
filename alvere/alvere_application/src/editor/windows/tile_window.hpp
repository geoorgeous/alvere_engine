#pragma once

#include <unordered_map>

#include "imgui/imgui.h"
#include "imgui_window.hpp"
#include "editor/tilemap/editor_tile.hpp"

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
	const ImColor m_highlightedBorder = { 0.2f, 0.2f, 0.2f };

	alvere::Vector2i m_gridSize;

	alvere::Vector2i m_selectedPosition;
	std::unordered_map<alvere::Vector2i, std::unique_ptr<EditorTile>, PositionHash> m_tilePositionMapping;

	std::unique_ptr<alvere::Texture> m_noTilePreview;

public:

	TileWindow();

	virtual void Draw() override;

	virtual std::string GetName() const override { return "Tile Palette"; }


	EditorTile * GetSelectedTile();

	Tile & GetOrAddTile(const Tile & tile);

private:

	void DrawTile(alvere::Vector2i position, alvere::Vector2i gridSize);

	void DrawInvalidTile(alvere::Vector2i position);
	void DrawValidTile(EditorTile & tile, alvere::Vector2i position);

	alvere::Vector2i GetFirstFreePosition();

	inline ImColor GetBorderColor(alvere::Vector2i position)
	{
		return m_selectedPosition == position
			? ImColor(1.0f, 1.0f, 0.0)
			: ImColor(0.0f, 0.0f, 0.0f);
	}
};