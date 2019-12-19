#pragma once

#include <alvere/graphics/texture.hpp>

#include "tilemap/tile.hpp"

class EditorTile
{
	std::unique_ptr<alvere::Texture> m_previewTexture;

public:

	EditorTile();
	EditorTile(Tile tile);

	Tile m_tile;

	alvere::Texture * GetPreviewTexture();
};