#pragma once

#include "alvere/world/entity/entity.hpp"
#include "alvere/utils/pool.hpp"
#include "alvere/utils/pool_handle.hpp"

namespace alvere
{
	using EntityHandle = Pool<Entity>::Handle;
}