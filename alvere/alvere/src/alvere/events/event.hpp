#pragma once

#include <functional>
#include <string>
#include <queue>

#define ALV_EVENT_BIND_FUNC(f) std::bind(&##f, this, std::placeholders::_1)

#define ALV_EVENT_CLASS_EVENTTYPE_FUNCS(type)\
	static Event::Type StaticGetEventType() { return type; }\
	Event::Type GetEventType() const override { return StaticGetEventType(); }\
	const char* GetName() const override { return #type; }

#define ALV_EVENT_CLASS_CATFLAGS(category_flags)\
	int GetCategoryFlags() const override { return category_flags; }

namespace alvere
{
	template<typename EventT>
	class EventHandler;

	template <typename ... EventArgs>
	class Event
	{
	public:

		using Function = std::function<void(EventArgs ...)>;

		void subscribe(Function f);

		void unsubscribe(Function f);

		void dispatch(EventArgs ...);

		void operator+=(Function f);

		void operator-=(Function f);

	private:

		std::queue<Function> m_subscribers;
	};
}