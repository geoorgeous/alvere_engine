
#include <alvere/utils/assets.hpp>
#include <alvere\world\component\components\c_transform.hpp>
#include <alvere\world\component\components\c_sprite.hpp>

#include "entity_definitions/def_player.hpp"
#include "components\physics\c_gravity.hpp"
#include "components\physics\c_tilemap_collision.hpp"
#include "components/physics/c_velocity.hpp"
#include "components/physics/c_movement.hpp"
#include "components/physics/c_friction.hpp"
#include "components/physics/c_collider.hpp"
#include "components/rendering/c_spritesheet.hpp"
#include "components/rendering/c_animation.hpp"
#include "components/c_direction.hpp"
#include "components/c_name.hpp"
#include "components/c_player.hpp"

using namespace alvere;

EntityHandle Def_Player::SpawnInstance(World & world)
{
	EntityHandle player = world.SpawnEntity<
		C_Player,
		C_Transform,
		C_Direction,
		C_Velocity,
		C_Friction,
		C_TilemapCollision,
		C_Gravity,
		C_Movement,
		C_Sprite,
		C_Spritesheet,
		C_Animation,
		C_Collider,
		C_Name
	>();

	alvere::Vector2 characterWorldSize(0.72f, 1.0f);

	{ //C_Name
		C_Name & name = world.GetComponent<C_Name>(player);
		name.m_Name = "Player";
	}

	{ //C_Collider
		//Single collider for the time being centered in the middle of the player's feet
		ColliderInstance collider;
		collider.m_LocalBounds = alvere::Rect(-characterWorldSize.x / 2.0f, 0.0f, characterWorldSize.x, characterWorldSize.y);

		C_Collider & colliderContainer = world.GetComponent<C_Collider>(player);
		colliderContainer.AddInstance(collider);
	}

	{ //C_Sprite
		Asset<Texture> textureAsset = AssetManager::getStatic<Texture>("res/img/player/player.png");

		C_Sprite & sprite = world.GetComponent<C_Sprite>(player);
		sprite.m_sprite = alvere::Sprite(*textureAsset, alvere::Rect(-characterWorldSize.x / 2.0f, 0.0f, characterWorldSize.x, characterWorldSize.y));
	}

	{ //C_Spritesheet
		C_Spritesheet & spritesheet = world.GetComponent<C_Spritesheet>(player);
		spritesheet.m_Offset = { 0, 0 };
		spritesheet.m_SourceRect = { 0, 0, 21, 29 };
	}

	{ //C_Animation
		C_Animation & animation = world.GetComponent<C_Animation>(player);

		{ //Idle
			C_Animation::Animation idle;
			idle.m_Loop = true;
			idle.m_Frames.emplace_back(C_Animation::Frame{ 3.2f, { 0, 4 } });
			idle.m_Frames.emplace_back(C_Animation::Frame{ 0.1f, { 1, 4 } });
			idle.m_Frames.emplace_back(C_Animation::Frame{ 0.1f, { 2, 4 } });
			idle.m_Frames.emplace_back(C_Animation::Frame{ 0.1f, { 3, 4 } });
			animation.Add("idle", idle);
		}
		
		animation.Start("idle");
	}

	return player;
}
