
#include <alvere/graphics/texture.hpp>
#include <alvere\world\component\components\c_transform.hpp>
#include <alvere\world\component\components\c_sprite.hpp>
#include <alvere/world/component/components/c_destroy.hpp>

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
	auto & transform = m_World.GetComponent<alvere::C_Transform>(player);

	//Have to make this static for now as the asset going out of scope deletes my texture reference
	static alvere::Asset<alvere::Texture> textureAsset = alvere::Texture::New("res/img/test.png");
	sprite.m_sprite = alvere::Sprite(*textureAsset.get());

	transform->setPosition(alvere::Vector3{ 1.5, 1.5, 0 });
	transform->setScale(alvere::Vector3{ 4, 4, 1 });

	scene->AddEntity(player);
}
