#pragma once

#include <memory>

namespace alvere
{
	class FrameBuffer
	{
	public:
		static std::unique_ptr<FrameBuffer> create(unsigned int width, unsigned int height);

		FrameBuffer(const FrameBuffer & frameBuffer);

		virtual ~FrameBuffer();

		virtual void setWidth(unsigned int newWidth) = 0;

		virtual void setHeight(unsigned int newHeight) = 0;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		inline unsigned int getWidth() const
		{
			return m_width;
		}

		inline unsigned int getHeight() const
		{
			return m_height;
		}

		inline float getAspect() const
		{
			return m_aspect;
		}

		FrameBuffer & operator=(const FrameBuffer & frameBuffer);

	protected:

		FrameBuffer();

		FrameBuffer(unsigned int width, unsigned int height);

		unsigned int m_width;

		unsigned int m_height;

		float m_aspect;
	};
}