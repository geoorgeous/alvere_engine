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

		Spritesheet * spritesheet = m_tile.m_spritesheet;

		if (spritesheet == nullptr)
		{
			return nullptr;
		}

		m_previewTexture = alvere::Texture::New(*spritesheet->GetTexture(), spritesheet->GetSourceRect({ 1, 1 }));

		return m_previewTexture.get();
	}
};