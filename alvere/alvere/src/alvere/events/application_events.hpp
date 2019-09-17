#pragma once

#include "alvere/events/event.hpp"

namespace alvere
{
	class WindowCloseEvent : public Event
	{
	public:
		ALV_EVENT_CLASS_EVENTTYPE_FUNCS(Event::Type::WindowClose);
		ALV_EVENT_CLASS_CATFLAGS(Event::Category::Application);
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height);

		ALV_EVENT_CLASS_EVENTTYPE_FUNCS(Event::Type::WindowResize);
		ALV_EVENT_CLASS_CATFLAGS(Event::Category::Application);

		std::string ToString() const override;

		inline unsigned int GetWidth() const
		{
			return m_Width;
		}

		inline unsigned int GetHeight() const
		{
			return m_Height;
		}

	private:
		unsigned int m_Width;
		unsigned int m_Height;
	};
}