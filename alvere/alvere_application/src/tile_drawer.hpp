#pragma once

#include <unordered_map>

#include <alvere/assets.hpp>
#include <alvere/graphics/sprite_batcher.hpp>
#include <alvere/graphics/texture.hpp>

#include "world_cell.hpp"

class TileDrawer
{
public:
	TileDrawer(const char * tileSheetFilePath);
	~TileDrawer();

	void DrawTile(alvere::SpriteBatcher& spriteBatcher, int x, int y, WorldCell::Type cellType);

private:
	alvere::Asset<alvere::Texture> m_TileSheet;
	alvere::RectI * m_TileSources;
};