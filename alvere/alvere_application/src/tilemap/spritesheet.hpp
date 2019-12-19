#pragma once

#include <alvere/utils/assets.hpp>
#include <alvere/graphics/texture.hpp>
#include <alvere/math/vectors.hpp>

struct Spritesheet
{
	alvere::Asset<alvere::Texture> m_texture;
	alvere::Vector2i m_tileSize;

	alvere::RectI GetSourceRect(alvere::Vector2i spritesheetCoord) const;

	bool operator==(const Spritesheet & rhs);
};