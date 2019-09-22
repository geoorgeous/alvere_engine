#include <sstream>

#include "alvere/events/application_events.hpp"

namespace alvere
{
	WindowResizeEvent::WindowResizeEvent(unsigned int width, unsigned int height)
		: m_width(width), m_height(height) { }

	std::string WindowResizeEvent::ToString() const
	{
		std::stringstream ss;
		ss << GetName() << ": width=" << m_width << ", height=" << m_height;
		return ss.str();
	}
}