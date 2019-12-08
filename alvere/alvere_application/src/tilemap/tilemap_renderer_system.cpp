#include "tilemap_renderer_system.hpp"
#include "tile.hpp"


TilemapRendererSystem::TilemapRendererSystem(alvere::Camera & camera)
	: m_camera(camera)
	, m_spriteBatcher(alvere::SpriteBatcher::New())
{
}

void TilemapRendererSystem::Render(C_Tilemap & tilemap)
{
	m_spriteBatcher->begin(m_camera.getProjectionViewMatrix());

	for (std::size_t y = 0; y < tilemap.m_size[1]; ++y)
	{
		for (std::size_t x = 0; x < tilemap.m_size[0]; ++x)
		{
			TileInstance & instance = tilemap.m_map[x + y * tilemap.m_size[0]];
			Spritesheet & spritesheet = *instance.m_tile->m_spritesheet;

			alvere::Rect position(x * tilemap.m_tileSize[0], y * tilemap.m_tileSize[1], tilemap.m_tileSize[0], tilemap.m_tileSize[1]);
			alvere::RectI sourceRect = spritesheet.GetSourceRect(instance.m_spritesheetCoordinate);
			const alvere::Asset<alvere::Texture> & texture = spritesheet.GetTexture();

			m_spriteBatcher->submit(texture.get(), position, sourceRect);
		}
	}

	m_spriteBatcher->end();
}