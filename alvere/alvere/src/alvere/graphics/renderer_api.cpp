#include "alvere/graphics/renderer_api.hpp"

#ifdef ALV_GRAPHICS_API_OPENGL
#include "graphics_api/opengl/opengl_renderer_api.hpp"
#endif

namespace alvere
{
#ifdef ALV_GRAPHICS_API_OPENGL
	Asset<RendererAPI> RendererAPI::s_Instance = Asset<graphics_api::opengl::RendererAPI>(new graphics_api::opengl::RendererAPI);
#endif
}