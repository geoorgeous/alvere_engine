#pragma once

#include <alvere/world/system/query_rendered_system.hpp>
#include <alvere\graphics\sprite_batcher.hpp>
#include <alvere\graphics\camera.hpp>

#include "c_tilemap.hpp"

class TilemapRendererSystem : public alvere::QueryRenderedSystem<C_Tilemap>
{
	alvere::Camera & m_camera;

	alvere::Asset<alvere::SpriteBatcher> m_spriteBatcher;

	alvere::Asset<alvere::Texture> m_fallbackTexture;

public:

	TilemapRendererSystem(alvere::Camera & camera);

	void Render(C_Tilemap & tilemap) override;
};