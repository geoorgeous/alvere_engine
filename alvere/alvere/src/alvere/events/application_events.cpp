#include <sstream>

#include "alvere/events/application_events.hpp"

namespace alvere
{
	WindowResizeEvent::WindowResizeEvent(unsigned int width, unsigned int height)
		: m_Width(width), m_Height(height) { }

	std::string WindowResizeEvent::ToString() const
	{
		std::stringstream ss;
		ss << GetName() << ": width=" << m_Width << ", height=" << m_Height;
		return ss.str();
	}
}