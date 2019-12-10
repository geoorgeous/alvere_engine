#pragma once

#include <memory>

#include "alvere/graphics/texture.hpp"
#include "alvere/math/vector/vec_2_i.hpp"

namespace alvere
{
	class FrameBuffer
	{
	public:
		static std::unique_ptr<FrameBuffer> create(unsigned int width, unsigned int height);

		FrameBuffer(const FrameBuffer & frameBuffer);

		virtual ~FrameBuffer() = 0;

		virtual void resize(int width, int height) = 0;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		inline Vec2i getSize() const
		{
			return m_texture->getDimensions();
		}

		inline float getAspect() const
		{
			return (float)m_texture->getDimensions().y / m_texture->getDimensions().x;
		}

		inline const Texture & getTexture() const
		{
			return *m_texture;
		}

		FrameBuffer & operator=(const FrameBuffer & frameBuffer);

	protected:

		FrameBuffer();

		std::unique_ptr<Texture> m_texture;
	};
}