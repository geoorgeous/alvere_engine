#include "alvere/application/application.hpp"

#include <chrono>
#include <memory>

#include <glfw/glfw3.h>

#include "alvere/debug/command_console/arg.hpp"
#include "alvere/debug/command_console/command_console.hpp"
#include "alvere/debug/command_console/command.hpp"
#include "alvere/debug/command_console/param.hpp"
#include "alvere/debug/exceptions.hpp"
#include "alvere/debug/logging.hpp"
#include "alvere/graphics/render_commands.hpp"

namespace alvere
{
	static std::unique_ptr<console::Command> s_quitCommand;

	Application::Application(const Window::Properties & properties)
		: m_window(Window::create(properties)), m_targetFrameRate(60.0f), m_running(true)
	{
		m_windowCloseEventHandler.setFunction([&]() {
			m_running = false;
		});
		*m_window->getEvent<WindowCloseEvent>() += m_windowCloseEventHandler;

		m_windowResizeEventHandler.setFunction([](int width, int height){
			console::gui::onWindowResize(width, height);
		});
		*m_window->getEvent<WindowResizeEvent>() += m_windowResizeEventHandler;

		console::gui::init(m_window.get());

		s_quitCommand = std::make_unique<console::Command>("quit", "Quits the application.", std::vector<console::IParam *>{}, [&](std::vector<const console::IArg *> args) -> CompositeText
		{
			m_running = false;
			return CompositeText(console::gui::defaultTextFormatting());
		});
	}

	void Application::run()
	{
		render_commands::setClearColour({0.1f, 0.1f, 0.1f, 1.0f});

		float timeStepNanoseconds = 1000000000.f / m_targetFrameRate;
		float timeStepSeconds = 1.0f / m_targetFrameRate;
		float deltaTime;
		float frameTimer = 1.0f;
		int framesThisSecond = 0;

		auto tickStartTime = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds timeStep((int)timeStepNanoseconds);
		std::chrono::nanoseconds deltaTimeChrono;
		std::chrono::nanoseconds lag(0);

		while (m_running)
		{
			framesThisSecond++;

			deltaTimeChrono = std::chrono::high_resolution_clock::now() - tickStartTime;
			tickStartTime = std::chrono::high_resolution_clock::now();
			lag += deltaTimeChrono;
			deltaTime = deltaTimeChrono.count() / 1000000000.0f;

			if ((frameTimer -= deltaTime) < 0.0f)
			{
				LogInfo("FPS: %i\n", framesThisSecond);

				framesThisSecond = 0;
				frameTimer = 1.0f;
			}

			try
			{
				while (lag >= timeStep)
				{
					lag -= timeStep;

					m_window->pollEvents();

					console::gui::update(timeStepSeconds);

					update(timeStepSeconds);
				}

				m_window->getRenderingContext().bindFrameBuffer();

				render();

				console::gui::draw();

				m_window->swapBuffers();
			}
			catch(FatalErrorException e)
			{
				LogError("%s\n", e.what());
				m_running = false;
			}
			catch(Exception e)
			{
				LogError("Runtime exception: %s\n", e.what());
			}
		}

		console::gui::destroy();

		m_window.release();

		glfwTerminate();
	}
}