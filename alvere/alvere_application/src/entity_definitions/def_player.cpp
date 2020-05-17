
#include <alvere\world\component\components\c_transform.hpp>
#include <alvere\world\component\components\c_sprite.hpp>

#include "entity_definitions/def_player.hpp"
#include "components\physics\c_gravity.hpp"
#include "components\physics\c_tilemap_collision.hpp"
#include "components/physics/c_velocity.hpp"
#include "components/c_player.hpp"
#include "components/physics/c_movement.hpp"
#include "components/physics/c_friction.hpp"

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
		C_Sprite
	>();

	//Have to make this static for now as the asset going out of scope deletes my texture reference
	static std::unique_ptr<Texture> textureAsset = Texture::New("res/img/test.png");

	C_Sprite & sprite = world.GetComponent<C_Sprite>(player);
	sprite.m_sprite = alvere::Sprite(*textureAsset.get());

	return player;
}
