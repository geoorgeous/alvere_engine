#pragma once

#include "alvere/assets.hpp"
#include "alvere/math/vectors.hpp"

namespace alvere
{
	class RendererAPI
	{
	public:
		static Asset<RendererAPI> s_Instance;

		virtual void SetClearColour(const alvere::Vector4& colour) = 0;

		virtual void Clear() = 0;
	};
}