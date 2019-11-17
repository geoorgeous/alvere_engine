
#include <alvere/graphics/texture.hpp>
#include <alvere\world\component\components\c_transform.hpp>
#include <alvere\world\component\components\c_sprite.hpp>

#include "platformer_scene.hpp"

std::unique_ptr<alvere::Scene> PlatformerScene::LoadScene()
{
	std::unique_ptr<alvere::Scene> scene = std::make_unique<alvere::Scene>();

	SpawnPlayer(scene);
	
	return std::move(scene);
}

void PlatformerScene::SpawnPlayer(std::unique_ptr<alvere::Scene>& scene)
{
	alvere::EntityHandle player = m_World.SpawnEntity<alvere::C_Transform, alvere::C_Sprite>();
	auto & sprite = m_World.GetComponent<alvere::C_Sprite>(player);

	//Have to make this static for now as the asset going out of scope deletes my texture reference
	static alvere::Asset<alvere::Texture> textureAsset = alvere::Texture::New("res/img/test.png");
	sprite.m_Sprite = alvere::Sprite(*textureAsset.get(), { 0, 0 });

	scene->AddEntity(player);
}
