#pragma once

#include <functional>
#include <string>

#define ALV_EVENT_BIND_FUNC(f) std::bind(&##f, this, std::placeholders::_1)

#define ALV_EVENT_CLASS_EVENTTYPE_FUNCS(type)\
	static Event::Type StaticGetEventType() { return type; }\
	Event::Type GetEventType() const override { return StaticGetEventType(); }\
	const char* GetName() const override { return #type; }

#define ALV_EVENT_CLASS_CATFLAGS(category_flags)\
	int GetCategoryFlags() const override { return category_flags; }

namespace alvere
{
	class Event
	{
	public:
		enum class Type
		{
			None = 0,
			WindowClose,
			WindowResize
		};

		enum Category
		{
			None = 0,
			Application = 1 << 0
		};

		bool m_Handled = false;

		virtual Type GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const;

		inline bool IsInCategory(Category category)
		{
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFunction = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
			: m_Event(event) { }

		template<typename T>
		bool Dispatch(EventFunction<T> function)
		{
			if (m_Event.GetEventType() == T::StaticGetEventType())
			{
				m_Event.m_Handled = function((T&)m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};
}