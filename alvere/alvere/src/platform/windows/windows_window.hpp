#pragma once

#include <unordered_map>
#include <utility>

#include "alvere/application/window.hpp"
#include "alvere/graphics/rendering_context.hpp"

namespace alvere::platform::windows
{
	class Window : public alvere::Window
	{
	public:

		Window(const alvere::Window::Properties& properties);

		~Window();

		void pollEvents() override;

		void disableCursor() override;

		void enableCursor() override;

		GLFWwindow * m_windowHandle;

	private:

		struct WindowUserPointerData
		{
			Window * window;
			Properties * properties;
			std::unordered_map<Key, std::pair<KeyData, KeyData>> * keys;
			MouseData * mouse;
		};

		WindowUserPointerData m_windowUserPointerData;

		void init(const alvere::Window::Properties& properties);

		void shutdown();
	};
}