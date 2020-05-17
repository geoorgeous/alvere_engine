#pragma once

#include "alvere/world/system/query_updated_system.hpp"
#include "alvere/world/archetype/archetype_query.hpp"
#include "alvere/world/component/components/c_transform.hpp"
#include "alvere/world/component/components/c_mover.hpp"
#include "alvere/debug/logging.hpp"

namespace alvere
{
	class S_Mover : public QueryUpdatedSystem<C_Transform, C_Mover>
	{
	public:

		virtual void Update(float deltaTime, C_Transform & transform, C_Mover & mover) override
		{
			//alvere::LogInfo( "Mover system being updated, Position is: %.1f!\n", transform.m_X );
			transform->move(Vector3(mover.m_Speed));
		}
	};
}