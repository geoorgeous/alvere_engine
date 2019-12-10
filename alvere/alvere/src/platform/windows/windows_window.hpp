#pragma once

#include <string>
#include <unordered_map>
#include <utility>

#include "alvere/application/window.hpp"
#include "alvere/graphics/rendering_context.hpp"

namespace alvere::platform::windows
{
	class Window : public alvere::Window
	{
	public:

		Window(const alvere::Window::Properties & properties, Vec2i resolution);

		~Window();

		void setTitle(std::string title) override;

		void setPosition(int x, int y) override;

		void resize(int width, int height) override;

		void setFlag(Flag flag, bool value) override;

		void maximize() override;

		void minimize() override;

		void restore() override;

		void focus() override;

		void requestAttention() override;

		void pollEvents() override;

		GLFWwindow * m_windowHandle;

	private:

		struct WindowUserPointerData
		{
			Window * window;
			std::unordered_map<Key, std::pair<KeyData, KeyData>> * keys;
			MouseData * mouse;
		};

		WindowUserPointerData m_windowUserPointerData;

		GLFWmonitor * m_monitorHandle;

		void init(const alvere::Window::Properties& properties, Vec2i resolution);

		void shutdown();
	};
}