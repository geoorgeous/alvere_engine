#include "alvere/world/system/systems/sprite_renderer_system.hpp"

namespace alvere
{
	SpriteRendererSystem::SpriteRendererSystem(Camera & camera)
		: m_camera(camera)
	{
		m_spriteBatcher = SpriteBatcher::New();
	}

	void SpriteRendererSystem::Render(World & world)
	{
		m_spriteBatcher->begin(m_camera.GetProjectionViewMatrix());

		QueryRenderedSystem<C_Transform, C_Sprite>::Render(world);

		m_spriteBatcher->end();
	}

	void SpriteRendererSystem::Render(C_Transform & transform, C_Sprite & sprite)
	{
		Rect destination = Rect{
			transform->getPosition().x,
			transform->getPosition().y,
			sprite.getTexture().width() * transform->getScale().x,
			sprite.getTexture().height() * transform->getScale().y
		};
		m_spriteBatcher->submit(&sprite.getTexture(), destination, sprite.textureSource(), sprite.colour());
	}
}