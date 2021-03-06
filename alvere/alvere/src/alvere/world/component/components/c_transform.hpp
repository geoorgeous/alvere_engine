#pragma once

#include "alvere/world/application/transform.hpp"
#include "alvere/world/component/pooled_component.hpp"

namespace alvere
{
	struct C_Transform : public PooledComponent<C_Transform>
	{
		Transform m_transform;

		inline Transform * operator->()
		{
			return &m_transform;
		}

		inline const Transform * operator->() const
		{
			return &m_transform;
		}

		virtual std::string to_string() const override
		{
			std::string str = "";

			alvere::Vector2 position = m_transform.getPosition();
			str += "x: " + std::to_string(position.x) + '\n';
			str += "y: " + std::to_string(position.y) + '\n';

			return str;
		}
	};
}