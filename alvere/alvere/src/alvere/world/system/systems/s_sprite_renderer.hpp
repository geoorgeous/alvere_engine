#pragma once

#include "alvere/graphics/camera.hpp"
#include "alvere/graphics/sprite_batcher.hpp"
#include "alvere/world/component/components/c_sprite.hpp"
#include "alvere/world/component/components/c_transform.hpp"
#include "alvere/world/system/query_rendered_system.hpp"

namespace alvere
{
	class S_SpriteRenderer : public virtual QueryRenderedSystem<const C_Transform, const C_Sprite>
	{
	public:

		S_SpriteRenderer(Camera & camera);

		virtual void Render(World & world) override;

		virtual void Render(const C_Transform & transform, const C_Sprite & sprite) override;

	private:

		std::unique_ptr<SpriteBatcher> m_spriteBatcher;

		Camera & m_camera;
	};
}