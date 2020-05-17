
#include <alvere\world\component\components\c_transform.hpp>
#include <alvere\world\component\components\c_sprite.hpp>
#include <alvere/world/component/components/c_camera.hpp>

#include "entity_definitions/def_camera.hpp"
#include "components\c_entity_follower.hpp"

using namespace alvere;

EntityHandle Def_Camera::SpawnInstance(World & world)
{
	EntityHandle camera = world.SpawnEntity<
		C_Transform,
		C_Camera,
		C_EntityFollower
	>();

	return camera;
}
