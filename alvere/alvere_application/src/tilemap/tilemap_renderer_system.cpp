#include "tilemap_renderer_system.hpp"
#include "tile.hpp"


TilemapRendererSystem::TilemapRendererSystem(alvere::Camera & camera)
	: m_camera(camera)
	, m_spriteBatcher(alvere::SpriteBatcher::New())
	, m_fallbackTexture(alvere::Texture::New("res/img/tiles/missing_tile.png"))
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

			alvere::Rect position(x * tilemap.m_tileSize[0], y * tilemap.m_tileSize[1], tilemap.m_tileSize[0], tilemap.m_tileSize[1]);

			if (instance.m_tile == nullptr)
			{
				//Cannot render a tile that doesn't exist, so instead render the fallback
				m_spriteBatcher->submit(m_fallbackTexture.get(), position);
				continue;
			}

			Spritesheet & spritesheet = instance.m_tile->m_spritesheet;

			alvere::RectI sourceRect = spritesheet.GetSourceRect(instance.m_spritesheetCoordinate);

			m_spriteBatcher->submit(spritesheet.m_texture.getAssetPtr(), position, sourceRect);
		}
	}

	m_spriteBatcher->end();
}