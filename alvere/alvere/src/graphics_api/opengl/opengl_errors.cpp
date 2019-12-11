#include "graphics_api/opengl/opengl_errors.hpp"

#include <glad/glad.h>

namespace alvere::graphics_api::opengl
{
	std::string GetOpenGLErrorString(GLenum err)
	{
		switch(err)
		{
			case GL_INVALID_ENUM:					return "Invalid enum: One or more of the given enumeration parameters is not a legal enumeration for this function.";
			case GL_INVALID_VALUE:					return "Invalid value: One or more of the given value parameters is not a legal value for this function.";
			case GL_INVALID_OPERATION:				return "Invalid operation: The set of state for this command is not legal for the parameters given to it or the combination of parameters given to it are not a legal combination.";
			case GL_INVALID_FRAMEBUFFER_OPERATION:  return "Invalid framebuffer operation: Attempted to read from or write/render to a framebuffer that is not complete.";
			case GL_OUT_OF_MEMORY:					return "Out of memory: Failed to allocate enough memory while completing this command.";
			case GL_NO_ERROR:						return "No error.";
			default: return "Unknown error enum!";
		}
	}
}