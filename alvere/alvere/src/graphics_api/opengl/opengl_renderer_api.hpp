#pragma once

#include "alvere/graphics/renderer_api.hpp"

namespace alvere::graphics_api::opengl
{
	class RendererAPI : public alvere::RendererAPI
	{
	public:
		void SetClearColour(const alvere::Vector4& colour) override;

		void Clear() override;
	};
}