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

		void setIcon(const std::vector<const Texture> & iconTextures) override;

		void setSizeLimits(int minWidth, int minHeight, int maxWidth, int maxHeight) override;

		void setCursor(CursorType cursorType) override;

		void maximize() override;

		void minimize() override;

		void restore() override;

		void focus() override;

		void requestAttention() override;

		void pollEvents() override;

		void swapBuffers() override;

		inline GLFWwindow * getHandle() const
		{
			return m_windowHandle;
		}

	private:

		struct WindowUserPointerData
		{
			Window * window;
			Vec2i * position;
			std::unordered_map<Key, std::pair<KeyData, KeyData>> * keys;
			MouseData * mouse;
			void (Window:: * windowResizeCallback)(int, int);
		};

		GLFWwindow * m_windowHandle;

		Vec2i m_windowedModePosition;

		Vec2i m_windowedModeSize;

		WindowUserPointerData m_windowUserPointerData;

		GLFWmonitor * m_monitorHandle;

		GLFWcursor * m_cursor;

		bool m_minimised;

		void init(const alvere::Window::Properties& properties, Vec2i resolution);

		void setCallbacks();

		void shutdown();

		void resizeCallback(int width, int height);
	};
}