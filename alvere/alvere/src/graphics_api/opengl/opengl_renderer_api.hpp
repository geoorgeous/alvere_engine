#pragma once

#include "alvere/graphics/renderer_api.hpp"

namespace alvere::graphics_api::opengl
{
	class RendererAPI : public alvere::RendererAPI
	{
	public:
		void setClearColour(const alvere::Vector4& colour) override;

		void clear() override;

		void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
	};
}