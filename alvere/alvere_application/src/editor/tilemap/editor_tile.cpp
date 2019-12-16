#include "editor/tilemap/editor_tile.hpp"


EditorTile::EditorTile()
{
}

EditorTile::EditorTile(Tile tile)
	: m_tile(tile)
{
}

alvere::Texture * EditorTile::GetPreviewTexture()
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