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
			transform->getScale().x * sprite.m_sprite.bounds().m_width,
			transform->getScale().y * sprite.m_sprite.bounds().m_height
		};

		alvere::RectI textureSource = sprite.m_sprite.textureSource();

		if (sprite.m_mirrored[0])
		{
			textureSource.m_x += textureSource.m_width;
			textureSource.m_width *= -1;
		}
		if (sprite.m_mirrored[1])
		{
			textureSource.m_y += textureSource.m_height;
			textureSource.m_height *= -1;
		}

		m_spriteBatcher->submit(&sprite.m_sprite.getTexture(), destination, textureSource, sprite.m_sprite.colour());
	}
}