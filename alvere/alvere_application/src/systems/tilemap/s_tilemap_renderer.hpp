#pragma once

#include <alvere/world/system/query_rendered_system.hpp>
#include <alvere\graphics\sprite_batcher.hpp>
#include <alvere\graphics\camera.hpp>

#include "components/tilemap/c_tilemap.hpp"

class S_TilemapRenderer : public alvere::QueryRenderedSystem<C_Tilemap>
{
	alvere::Camera & m_camera;

	std::unique_ptr<alvere::SpriteBatcher> m_spriteBatcher;

	std::unique_ptr<alvere::Texture> m_fallbackTexture;

public:

	S_TilemapRenderer(alvere::Camera & camera);

	void Render(C_Tilemap & tilemap) override;
};