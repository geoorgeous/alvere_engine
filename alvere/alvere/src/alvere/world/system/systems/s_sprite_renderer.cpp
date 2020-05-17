#include "alvere/world/system/systems/s_sprite_renderer.hpp"

namespace alvere
{
	S_SpriteRenderer::S_SpriteRenderer(Camera & camera)
		: m_camera(camera)
	{
		m_spriteBatcher = SpriteBatcher::New();
	}

	void S_SpriteRenderer::Render(World & world)
	{
		m_spriteBatcher->begin(m_camera.getProjectionViewMatrix());

		QueryRenderedSystem<C_Transform, C_Sprite>::Render(world);

		m_spriteBatcher->end();
	}

	void S_SpriteRenderer::Render(C_Transform & transform, C_Sprite & sprite)
	{
		Rect destination = Rect{
			transform->getPosition().x + sprite.m_sprite.bounds().m_x,
			transform->getPosition().y + sprite.m_sprite.bounds().m_y,
			transform->getScale().x,
			transform->getScale().y
		};
		m_spriteBatcher->submit(&sprite.m_sprite.getTexture(), destination, sprite.m_sprite.textureSource(), sprite.m_sprite.colour());
	}
}