#pragma once

#include <alvere/graphics/texture.hpp>

#include "tilemap/tile.hpp"

class EditorTile
{
	alvere::Asset<alvere::Texture> m_previewTexture;

public:

	Tile m_tile;

	std::string m_texturePath;

	alvere::Texture * GetPreviewTexture()
	{
		if (m_previewTexture)
		{
			return m_previewTexture.get();
		}

		if (m_tile.m_spritesheet.m_texture == false)
		{
			return nullptr;
		}

		m_previewTexture = alvere::Texture::New(*m_tile.m_spritesheet.m_texture, m_tile.m_spritesheet.GetSourceRect({ 1, 1 }));

		return m_previewTexture.get();
	}
};