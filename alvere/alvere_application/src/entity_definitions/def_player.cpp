
#include <alvere\world\component\components\c_transform.hpp>
#include <alvere\world\component\components\c_sprite.hpp>

#include "entity_definitions/def_player.hpp"
#include "components\c_gravity.hpp"

alvere::EntityHandle Def_Player::SpawnInstance(alvere::World & world)
{
	alvere::EntityHandle player = world.SpawnEntity<
		alvere::C_Transform,
		alvere::C_Sprite,
		C_Gravity
	>();

	//Have to make this static for now as the asset going out of scope deletes my texture reference
	static std::unique_ptr<alvere::Texture> textureAsset = alvere::Texture::New("res/img/test.png");

	alvere::C_Sprite & sprite = world.GetComponent<alvere::C_Sprite>(player);
	sprite.m_sprite = alvere::Sprite(*textureAsset.get());

	//alvere::C_Transform & transform = m_World.GetComponent<alvere::C_Transform>(player);
	//transform->setScale(alvere::Vector3{ 4, 4, 1 });

	return player;
}
