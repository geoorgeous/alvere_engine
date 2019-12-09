#include "alvere/graphics/renderer_api.hpp"

#ifdef ALV_GRAPHICS_API_OPENGL
#include "graphics_api/opengl/opengl_renderer_api.hpp"
#endif

namespace alvere
{
#ifdef ALV_GRAPHICS_API_OPENGL
	std::unique_ptr<RendererAPI> RendererAPI::s_Instance = std::make_unique<graphics_api::opengl::RendererAPI>();
#endif
}