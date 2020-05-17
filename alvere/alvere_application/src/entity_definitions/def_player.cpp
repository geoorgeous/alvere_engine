
#include <alvere\world\component\components\c_transform.hpp>
#include <alvere\world\component\components\c_sprite.hpp>

#include "entity_definitions/def_player.hpp"
#include "components\physics\c_gravity.hpp"
#include "components\physics\c_tilemap_collision.hpp"
#include "components/physics/c_velocity.hpp"
#include "components/physics/c_movement.hpp"
#include "components/physics/c_friction.hpp"
#include "components/physics/c_collider.hpp"
#include "components/c_player.hpp"

using namespace alvere;

EntityHandle Def_Player::SpawnInstance(World & world)
{
	EntityHandle player = world.SpawnEntity<
		C_Player,
		C_Transform,
		C_Velocity,
		C_Friction,
		C_TilemapCollision,
		C_Gravity,
		C_Movement,
		C_Sprite,
		C_Collider
	>();

	{
		//Single collider for the time being centered in the middle of the player's feet
		ColliderInstance collider;
		collider.m_LocalBounds = alvere::Rect( -0.5f, 0.0f, 1.0f, 1.0f );

		C_Collider & colliderContainer = world.GetComponent<C_Collider>(player);
		colliderContainer.AddInstance(collider);
	}
	
	{
		//Have to make this static for now as the asset going out of scope deletes my texture reference
		static std::unique_ptr<Texture> textureAsset = Texture::New("res/img/test.png");

		C_Sprite & sprite = world.GetComponent<C_Sprite>(player);
		sprite.m_sprite = alvere::Sprite(*textureAsset.get());
	}

	return player;
}
