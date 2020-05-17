#include "systems/rendering/s_collider_renderer.hpp"

S_ColliderRenderer::S_ColliderRenderer(alvere::Camera & camera, const alvere::Sprite & sprite)
	: m_camera(camera)
	, m_Sprite(sprite)
{
	m_spriteBatcher = alvere::SpriteBatcher::New();
}

void S_ColliderRenderer::Render(alvere::World & world)
{
	m_spriteBatcher->begin(m_camera.getProjectionViewMatrix());

	QueryRenderedSystem<const alvere::C_Transform, const C_Collider>::Render(world);

	m_spriteBatcher->end();
}

void S_ColliderRenderer::Render(const alvere::C_Transform & transform, const C_Collider & collider)
{
	for (const ColliderInstance & colliderInstance : collider.m_ColliderInstances)
	{
		alvere::Vector2 topRight = colliderInstance.m_LocalBounds.getTopRight();
		alvere::Vector2 bottomLeft = colliderInstance.m_LocalBounds.getBottomLeft();

		alvere::Rect destination = alvere::Rect
		{
			alvere::Vector2(transform->getPosition()) + bottomLeft,
			topRight - bottomLeft
		};

		m_spriteBatcher->submit(&m_Sprite.getTexture(), destination, m_Sprite.textureSource(), m_Sprite.colour());
	}
}