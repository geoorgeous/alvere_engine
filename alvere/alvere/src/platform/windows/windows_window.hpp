#pragma once

#include <unordered_map>

#include "alvere/application/window.hpp"
#include "alvere/graphics/rendering_context.hpp"

namespace alvere::platform::windows
{
	class Window : public alvere::Window
	{
	public:
		Window(const alvere::Window::Properties& properties);
		~Window();

		void PollEvents() override;
		void SwapBuffers() override;
		void DisableCursor() override;
		void EnableCursor() override;
		inline void SetEventCallback(const EventCallbackFunction& callback) override { m_EventCallback = callback; }

	private:
		struct WindowUserPointerData
		{
			Window * window;
			std::unordered_map<Key, KeyData> * keys;
			MouseData * mouse;
		};

		GLFWwindow * m_WindowHandle;
		WindowUserPointerData m_WindowuserPointerData;
		alvere::RenderingContext * m_RenderingContext;

		void Init(const alvere::Window::Properties& properties);
		void Shutdown();
	};
}