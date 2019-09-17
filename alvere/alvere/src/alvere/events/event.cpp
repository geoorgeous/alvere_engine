#include "alvere/events/event.hpp"

namespace alvere
{
	std::string Event::ToString() const
	{
		return GetName();
	}
}