#pragma once

#include "alvere/application/window.hpp"
#include "alvere/assets.hpp"

#define ALV_NEW_APP_FUNC(app_class_name) alvere::Application * alvere::NewApplication() { return new app_class_name(); }

namespace alvere
{
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;
		virtual void update(float deltaTime) = 0;
		virtual void render() = 0;
		void run();

	protected:
		Asset<Window> m_window;
		float m_targetFrameRate;

	private:
		bool m_running;

		void onEvent(Event& event);
		bool onWindowClose(WindowCloseEvent& e);
	};

	extern Application * NewApplication();
}