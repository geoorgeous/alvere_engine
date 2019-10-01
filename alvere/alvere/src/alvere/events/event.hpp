#pragma once

#include <functional>
#include <string>
#include <vector>

#define ALV_EVENT_BIND_FUNC(f) std::bind(&##f, this, std::placeholders::_1)

#define ALV_EVENT_CLASS_EVENTTYPE_FUNCS(type)\
	static Event::Type StaticGetEventType() { return type; }\
	Event::Type GetEventType() const override { return StaticGetEventType(); }\
	const char* GetName() const override { return #type; }

#define ALV_EVENT_CLASS_CATFLAGS(category_flags)\
	int GetCategoryFlags() const override { return category_flags; }

namespace alvere
{
	class EventBase { };

	template <typename ... EventArgs>
	class Event : public EventBase
	{
	public:

		using Function = std::function<void(EventArgs ...)>;

		class Handler
		{
		public:

			friend class Event<EventArgs ...>;
			
			Handler() { }

			Handler(Function f)
			{
				m_handlerFunction = f;
			}

			void setFunction(Function f)
			{
				m_handlerFunction = f;
			}

			bool operator==(const Handler & other)
			{
				return this == &other;
			}

		private:

			Function m_handlerFunction;
		};

		virtual ~Event<EventArgs ...>() = 0 { };

		void subscribe(const Handler & handler)
		{
			if (std::find(m_subscribers.begin(), m_subscribers.end(), &handler) != m_subscribers.end())
				return;

			m_subscribers.emplace_back(&handler);
		}

		void unsubscribe(const Handler & handler)
		{
			auto iter = std::find(m_subscribers.begin(), m_subscribers.end(), &handler);

			if (iter == m_subscribers.end())
				return;

			m_subscribers.erase(iter);
		}

		void dispatch(EventArgs ... args)
		{
			for (const Handler * handler : m_subscribers)
				handler->m_handlerFunction(args ...);
		}

		void operator+=(const Handler & handler)
		{
			subscribe(handler);
		}

		void operator-=(const Handler & handler)
		{
			unsubscribe(handler);
		}

	private:

		std::vector<const Handler *> m_subscribers;
	};
}