#include "tile_drawer.hpp"

TileDrawer::TileDrawer(const char * tileSheetFilepath)
{
	m_TileSheet = alvere::Texture::New(tileSheetFilepath);

	int textureHeight = m_TileSheet->height();
	m_TileSources = new alvere::RectI[10] {
		alvere::RectI(0, textureHeight - 16, 16, 16),

		alvere::RectI(0, textureHeight - 32, 16, 16),
		alvere::RectI(16, textureHeight - 32, 16, 16),
		alvere::RectI(32, textureHeight - 32, 16, 16),
		alvere::RectI(48, textureHeight - 32, 16, 16),

		alvere::RectI(64, textureHeight - 32, 16, 16),
		alvere::RectI(80, textureHeight - 32, 16, 16),
		alvere::RectI(96, textureHeight - 32, 16, 16),
		alvere::RectI(112, textureHeight - 32, 16, 16),

		alvere::RectI(0, textureHeight - 48, 16, 16)
	};
}

TileDrawer::~TileDrawer()
{
	delete[] m_TileSources;
}

void TileDrawer::DrawTile(alvere::SpriteBatcher& spriteBatcher, int x, int y, WorldCell::Type cellType)
{
	spriteBatcher.submit(m_TileSheet.get(), alvere::Rect{ (float)x, (float)y, 1, 1 }, m_TileSources[(int)cellType]);
}