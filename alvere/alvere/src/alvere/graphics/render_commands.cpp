#include "alvere/graphics/render_commands.hpp"
#include "alvere/graphics/renderer_api.hpp"

namespace alvere::render_commands
{
	void SetClearColour(const alvere::Vector4& colour)
	{
		RendererAPI::s_Instance->SetClearColour(colour);
	}

	void Clear()
	{
		RendererAPI::s_Instance->Clear();
	}
}