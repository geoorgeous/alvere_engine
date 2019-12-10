#pragma once

#include <memory>

#include "alvere/math/vectors.hpp"

namespace alvere
{
	class RendererAPI
	{
	public:
		static std::unique_ptr<RendererAPI> s_Instance;

		virtual void setClearColour(const alvere::Vector4& colour) = 0;

		virtual void clear() = 0;

		virtual void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;
	};
}