#pragma once

#include "alvere/graphics/camera.hpp"
#include "alvere/graphics/sprite_batcher.hpp"
#include "alvere/world/component/components/c_sprite.hpp"
#include "alvere/world/component/components/c_transform.hpp"
#include "alvere/world/system/query_rendered_system.hpp"

namespace alvere
{
	class SpriteRendererSystem : public virtual QueryRenderedSystem<C_Transform, C_Sprite>
	{
	public:

		SpriteRendererSystem(Camera & camera);

		virtual void Render(World & world) override;

		virtual void Render(C_Transform & transform, C_Sprite & sprite) override;

	private:

		Asset<SpriteBatcher> m_spriteBatcher;

		Camera & m_camera;
	};
}