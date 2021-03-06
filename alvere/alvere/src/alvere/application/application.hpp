#pragma once

#include "alvere/application/window.hpp"

#define ALV_NEW_APP_FUNC(app_class_name) alvere::Application * alvere::NewApplication() { return new app_class_name(); }

namespace alvere
{
	class Application
	{
	public:

		Application(const Window::Properties & properties = Window::Properties::s_default);

		virtual ~Application() = default;

		virtual void update(float deltaTime) = 0;

		virtual void render() = 0;

		void run();

	protected:

		std::unique_ptr<Window> m_window;

		float m_targetFrameRate;

	private:

		bool m_running;

		WindowCloseEvent::Handler m_windowCloseEventHandler;
		WindowResizeEvent::Handler m_windowResizeEventHandler;
	};

	extern Application * NewApplication();
}