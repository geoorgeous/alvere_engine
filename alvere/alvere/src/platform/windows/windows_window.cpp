#include <glfw/glfw3.h>

#include "alvere/utils/exceptions.hpp"
#include "alvere/events/application_events.hpp"
#include "graphics_api/opengl/opengl_context.hpp"
#include "platform/windows/windows_window.hpp"

namespace alvere::platform::windows
{
	static bool s_GLFWInitialised = false;

	Window::Window(const alvere::Window::Properties& properties)
	{
		Init(properties);
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::PollEvents()
	{
		m_Mouse.delta = Vector2::zero;

		glfwPollEvents();
	}

	void Window::SwapBuffers()
	{
		m_RenderingContext->SwapBuffers();
	}

	void Window::DisableCursor()
	{
		glfwSetInputMode(m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void Window::EnableCursor()
	{
		glfwSetInputMode(m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Window::Init(const alvere::Window::Properties& properties)
	{
		if (!s_GLFWInitialised)
		{
			if (!glfwInit())
			{
				AlvThrowFatal("Fatal error! Failed to initialise GLFW.");
			}

			glfwSetErrorCallback([](int error, const char* message)
				{
					AlvThrow("GLFW error (%i): Message: \"%s\".", error, message);
				});

			s_GLFWInitialised = true;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		m_WindowHandle = glfwCreateWindow((int)properties.sizeWidth, (int)properties.sizeHeight, properties.title.c_str(), nullptr, nullptr);

		if (m_WindowHandle == NULL)
		{
			AlvThrowFatal("Fatal error! Failed to create GLFW window.");
		}

		m_RenderingContext = new alvere::graphics_api::opengl::RenderingContext(m_WindowHandle);
		m_RenderingContext->Init();

		glfwSwapInterval(0);

		m_WindowuserPointerData = { this, &m_Keys, &m_Mouse };
		glfwSetWindowUserPointer(m_WindowHandle, &m_WindowuserPointerData);

		glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow * windowHandle)
		{
			Window & window = *((WindowUserPointerData *)glfwGetWindowUserPointer(windowHandle))->window;
			alvere::WindowCloseEvent event;
			window.m_EventCallback(event);
		});
		glfwSetWindowSizeCallback(m_WindowHandle, [](GLFWwindow * windowHandle, int width, int height)
		{
			Window & window = *((WindowUserPointerData *)glfwGetWindowUserPointer(windowHandle))->window;
			alvere::WindowResizeEvent event = alvere::WindowResizeEvent((unsigned int)width, (unsigned int)height);
			window.m_EventCallback(event);
		});
		glfwSetKeyCallback(m_WindowHandle, [](GLFWwindow * windowHandle, int key, int scancode, int action, int mods)
		{
			std::unordered_map<Key, KeyData> & keys = *((WindowUserPointerData *)glfwGetWindowUserPointer(windowHandle))->keys;

			if (action == GLFW_PRESS)
			{
				LogInfo("pressed\n");
			}
			else if (action == GLFW_RELEASE)
			{
				LogInfo("release\n");
			}
			else if (action == GLFW_REPEAT)
			{
				LogInfo("held/repeat\n");
			}

			auto SetKeyData = [](KeyData & keyData, int action)
			{
				keyData.isDown = action != GLFW_RELEASE;
				keyData.isRepeating = action == GLFW_REPEAT;
				keyData.justPressed = action == GLFW_PRESS;
				keyData.justReleased = action == GLFW_RELEASE;
			};

			switch (key)
			{
			case GLFW_KEY_ESCAPE: SetKeyData(keys[Key::Escape], action); break;
			case GLFW_KEY_GRAVE_ACCENT: SetKeyData(keys[Key::BackQuote], action); break;
			case GLFW_KEY_TAB: SetKeyData(keys[Key::Tab], action); break;
			case GLFW_KEY_CAPS_LOCK: SetKeyData(keys[Key::CapsLock], action); break;
			case GLFW_KEY_SCROLL_LOCK: SetKeyData(keys[Key::ScrollLock], action); break;
			case GLFW_KEY_NUM_LOCK: SetKeyData(keys[Key::NumLock], action); break;
			case GLFW_KEY_LEFT_SHIFT: SetKeyData(keys[Key::LeftShift], action); break;
			case GLFW_KEY_LEFT_CONTROL: SetKeyData(keys[Key::LeftCtrl], action); break;
			case GLFW_KEY_LEFT_ALT: SetKeyData(keys[Key::LeftAlt], action); break;
			case GLFW_KEY_SPACE: SetKeyData(keys[Key::Space], action); break;
			case GLFW_KEY_RIGHT_ALT: SetKeyData(keys[Key::RightAlt], action); break;
			case GLFW_KEY_RIGHT_CONTROL: SetKeyData(keys[Key::RightCtrl], action); break;
			case GLFW_KEY_RIGHT_SHIFT: SetKeyData(keys[Key::RightShift], action); break;
			case GLFW_KEY_ENTER: SetKeyData(keys[Key::Enter], action); break;
			case GLFW_KEY_BACKSPACE: SetKeyData(keys[Key::BackSpace], action); break;

			case GLFW_KEY_A: SetKeyData(keys[Key::A], action); break;
			case GLFW_KEY_B: SetKeyData(keys[Key::B], action); break;
			case GLFW_KEY_C: SetKeyData(keys[Key::C], action); break;
			case GLFW_KEY_D: SetKeyData(keys[Key::D], action); break;
			case GLFW_KEY_E: SetKeyData(keys[Key::E], action); break;
			case GLFW_KEY_F: SetKeyData(keys[Key::F], action); break;
			case GLFW_KEY_G: SetKeyData(keys[Key::G], action); break;
			case GLFW_KEY_H: SetKeyData(keys[Key::H], action); break;
			case GLFW_KEY_I: SetKeyData(keys[Key::I], action); break;
			case GLFW_KEY_J: SetKeyData(keys[Key::J], action); break;
			case GLFW_KEY_K: SetKeyData(keys[Key::K], action); break;
			case GLFW_KEY_L: SetKeyData(keys[Key::L], action); break;
			case GLFW_KEY_M: SetKeyData(keys[Key::M], action); break;
			case GLFW_KEY_N: SetKeyData(keys[Key::N], action); break;
			case GLFW_KEY_O: SetKeyData(keys[Key::O], action); break;
			case GLFW_KEY_P: SetKeyData(keys[Key::P], action); break;
			case GLFW_KEY_Q: SetKeyData(keys[Key::Q], action); break;
			case GLFW_KEY_R: SetKeyData(keys[Key::R], action); break;
			case GLFW_KEY_S: SetKeyData(keys[Key::S], action); break;
			case GLFW_KEY_T: SetKeyData(keys[Key::T], action); break;
			case GLFW_KEY_U: SetKeyData(keys[Key::U], action); break;
			case GLFW_KEY_V: SetKeyData(keys[Key::V], action); break;
			case GLFW_KEY_W: SetKeyData(keys[Key::W], action); break;
			case GLFW_KEY_X: SetKeyData(keys[Key::X], action); break;
			case GLFW_KEY_Y: SetKeyData(keys[Key::Y], action); break;
			case GLFW_KEY_Z: SetKeyData(keys[Key::Z], action); break;

			case GLFW_KEY_0: SetKeyData(keys[Key::D0], action); break;
			case GLFW_KEY_1: SetKeyData(keys[Key::D1], action); break;
			case GLFW_KEY_2: SetKeyData(keys[Key::D2], action); break;
			case GLFW_KEY_3: SetKeyData(keys[Key::D3], action); break;
			case GLFW_KEY_4: SetKeyData(keys[Key::D4], action); break;
			case GLFW_KEY_5: SetKeyData(keys[Key::D5], action); break;
			case GLFW_KEY_6: SetKeyData(keys[Key::D6], action); break;
			case GLFW_KEY_7: SetKeyData(keys[Key::D7], action); break;
			case GLFW_KEY_8: SetKeyData(keys[Key::D8], action); break;
			case GLFW_KEY_9: SetKeyData(keys[Key::D9], action); break;

			case GLFW_KEY_F1: SetKeyData(keys[Key::F1], action); break;
			case GLFW_KEY_F2: SetKeyData(keys[Key::F2], action); break;
			case GLFW_KEY_F3: SetKeyData(keys[Key::F3], action); break;
			case GLFW_KEY_F4: SetKeyData(keys[Key::F4], action); break;
			case GLFW_KEY_F5: SetKeyData(keys[Key::F5], action); break;
			case GLFW_KEY_F6: SetKeyData(keys[Key::F6], action); break;
			case GLFW_KEY_F7: SetKeyData(keys[Key::F7], action); break;
			case GLFW_KEY_F8: SetKeyData(keys[Key::F8], action); break;
			case GLFW_KEY_F9: SetKeyData(keys[Key::F9], action); break;
			case GLFW_KEY_F10: SetKeyData(keys[Key::F10], action); break;
			case GLFW_KEY_F11: SetKeyData(keys[Key::F11], action); break;
			case GLFW_KEY_F12: SetKeyData(keys[Key::F12], action); break;

			case GLFW_KEY_PRINT_SCREEN: SetKeyData(keys[Key::PrintScreen], action); break;
			case GLFW_KEY_PAUSE: SetKeyData(keys[Key::Pause], action); break;
			case GLFW_KEY_INSERT: SetKeyData(keys[Key::Insert], action); break;
			case GLFW_KEY_HOME: SetKeyData(keys[Key::Home], action); break;
			case GLFW_KEY_DELETE: SetKeyData(keys[Key::Delete], action); break;
			case GLFW_KEY_END: SetKeyData(keys[Key::End], action); break;
			case GLFW_KEY_PAGE_UP: SetKeyData(keys[Key::PageUp], action); break;
			case GLFW_KEY_PAGE_DOWN: SetKeyData(keys[Key::Down], action); break;

			case GLFW_KEY_LEFT: SetKeyData(keys[Key::Left], action); break;
			case GLFW_KEY_UP: SetKeyData(keys[Key::Up], action); break;
			case GLFW_KEY_DOWN: SetKeyData(keys[Key::Down], action); break;
			case GLFW_KEY_RIGHT: SetKeyData(keys[Key::Right], action); break;

			case GLFW_KEY_KP_DIVIDE: SetKeyData(keys[Key::NumPad_Divide], action); break;
			case GLFW_KEY_KP_MULTIPLY: SetKeyData(keys[Key::NumPad_Multiply], action); break;
			case GLFW_KEY_KP_SUBTRACT: SetKeyData(keys[Key::NumPad_Subtract], action); break;
			case GLFW_KEY_KP_ADD: SetKeyData(keys[Key::NumPad_Plus], action); break;
			case GLFW_KEY_KP_ENTER: SetKeyData(keys[Key::NumPad_Plus], action); break;
			case GLFW_KEY_KP_DECIMAL: SetKeyData(keys[Key::NumPad_Delete], action); break;
			case GLFW_KEY_KP_0: SetKeyData(keys[Key::NumPad_0], action); break;
			case GLFW_KEY_KP_1: SetKeyData(keys[Key::NumPad_1], action); break;
			case GLFW_KEY_KP_2: SetKeyData(keys[Key::NumPad_2], action); break;
			case GLFW_KEY_KP_3: SetKeyData(keys[Key::NumPad_3], action); break;
			case GLFW_KEY_KP_4: SetKeyData(keys[Key::NumPad_4], action); break;
			case GLFW_KEY_KP_5: SetKeyData(keys[Key::NumPad_5], action); break;
			case GLFW_KEY_KP_6: SetKeyData(keys[Key::NumPad_6], action); break;
			case GLFW_KEY_KP_7: SetKeyData(keys[Key::NumPad_7], action); break;
			case GLFW_KEY_KP_8: SetKeyData(keys[Key::NumPad_8], action); break;
			case GLFW_KEY_KP_9: SetKeyData(keys[Key::NumPad_9], action); break;
			}
		});
		glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow* windowHandle, double xPos, double yPos)
		{
			MouseData & mouse = *((WindowUserPointerData *)glfwGetWindowUserPointer(windowHandle))->mouse;
			mouse.delta.x = xPos - mouse.position.x;
			mouse.delta.y = yPos - mouse.position.y;
			mouse.position.x = xPos;
			mouse.position.y = yPos;
		});
		glfwSetMouseButtonCallback(m_WindowHandle, [](GLFWwindow * windowHandle, int button, int action, int mods)
		{
			MouseData & mouse = *((WindowUserPointerData *)glfwGetWindowUserPointer(windowHandle))->mouse;

			bool isDown = action == GLFW_PRESS;

			switch (button)
			{
			case GLFW_MOUSE_BUTTON_LEFT: mouse.buttons[MouseButton::Left] = isDown; break;
			case GLFW_MOUSE_BUTTON_RIGHT: mouse.buttons[MouseButton::Right] = isDown; break;
			case GLFW_MOUSE_BUTTON_MIDDLE: mouse.buttons[MouseButton::Middle] = isDown; break;
			}
		});
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_WindowHandle);
	}
}

alvere::Asset<alvere::Window> alvere::Window::New(const alvere::Window::Properties& properties)
{
	return alvere::Asset<alvere::platform::windows::Window>(new alvere::platform::windows::Window(properties));
}