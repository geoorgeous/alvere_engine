#pragma once

#include <sstream>
#include <string>

#include <glad/glad.h>

#include "alvere/debug/logging.hpp"

#ifdef NDEBUG
#define ALV_LOG_OPENGL_CALL(call) call
#else
#define ALV_LOG_OPENGL_CALL(call)\
	{\
		while (glGetError() != GL_NO_ERROR); \
		call; \
		GLenum err;\
		if ((err = glGetError()) != GL_NO_ERROR)\
			::alvere::LogError("OpenGL call '%s' caused '%s' error in %s, line %i.\n", #call, ::alvere::graphics_api::opengl::GetOpenGLErrorString(err).c_str(), __FILE__, __LINE__);\
	}
#endif

namespace alvere::graphics_api::opengl
{
	std::string GetOpenGLErrorString(GLenum err);
}