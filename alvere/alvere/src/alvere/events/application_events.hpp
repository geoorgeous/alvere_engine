#pragma once

#include "alvere/events/event.hpp"

namespace alvere
{
	class WindowCloseEvent : public Event<> { };

	class WindowResizeEvent : public Event<int, int> { };

	class CharInputEvent : public Event<unsigned int> { };
}