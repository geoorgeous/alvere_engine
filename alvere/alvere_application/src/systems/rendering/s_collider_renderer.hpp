#pragma once

#include <alvere/graphics/sprite.hpp>
#include <alvere/graphics/camera.hpp>
#include <alvere/graphics/sprite_batcher.hpp>
#include <alvere/world/component/components/c_transform.hpp>
#include <alvere/world/system/query_rendered_system.hpp>

#include "components/physics/c_collider.hpp"

class S_ColliderRenderer : public alvere::QueryRenderedSystem<alvere::C_Transform, C_Collider>
{
	std::unique_ptr<alvere::SpriteBatcher> m_spriteBatcher;

	alvere::Camera & m_camera;

	alvere::Sprite m_Sprite;

public:

	S_ColliderRenderer(alvere::Camera & camera, const alvere::Sprite & sprite);

	virtual void Render(alvere::World & world) override;

	virtual void Render(alvere::C_Transform & transform, C_Collider & collider) override;

};