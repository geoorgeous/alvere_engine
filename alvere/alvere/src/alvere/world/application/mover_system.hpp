#pragma once

#include "alvere/world/system/query_updated_system.hpp"
#include "alvere/world/archetype/archetype_query.hpp"
#include "alvere/world/application/c_transform.hpp"
#include "alvere/world/application/c_mover.hpp"
#include "alvere/world/application/c_saveable.hpp"
#include "alvere/utils/logging.hpp"

namespace alvere
{
	class MoverSystem : public QueryUpdatedSystem<C_Transform, C_Mover>
	{
	public:

		virtual void Update(C_Transform & transform, C_Mover & mover) override
		{
			//alvere::LogInfo( "Mover system being updated, Position is: %.1f!\n", transform.m_X );
			transform.m_X += mover.m_Speed;
		}
	};
}