#include <glfw/glfw3.h>

#include "alvere/utils/exceptions.hpp"
#include "alvere/events/application_events.hpp"
#include "graphics_api/opengl/opengl_context.hpp"
#include "platform/windows/windows_window.hpp"

namespace alvere::platform::windows
{
	static bool s_GLFWInitialised = false;

	Window::Window(const alvere::Window::Properties & properties)
		: alvere::Window(properties)
	{
		init(properties);
	}

	Window::~Window()
	{
		shutdown();
	}

	void Window::pollEvents()
	{
		for (auto & keyDataPair : m_keys)
		{
			keyDataPair.second.second = keyDataPair.second.first;
			keyDataPair.second.first.justPressed = false;
			keyDataPair.second.first.justReleased = false;
		}

		m_mouse.delta = Vector2::zero;
		m_mouse.scrollDelta = Vector2::zero;

		glfwPollEvents();
	}

	void Window::disableCursor()
	{
		glfwSetInputMode(m_windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void Window::enableCursor()
	{
		glfwSetInputMode(m_windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Window::init(const alvere::Window::Properties& properties)
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
		m_windowHandle = glfwCreateWindow((int)properties.sizeWidth, (int)properties.sizeHeight, properties.title.c_str(), nullptr, nullptr);

		if (m_windowHandle == NULL)
		{
			AlvThrowFatal("Fatal error! Failed to create GLFW window.");
		}

		m_renderingContext = new alvere::graphics_api::opengl::RenderingContext(m_windowHandle);
		m_renderingContext->init();

		glfwSwapInterval(0);

		m_windowUserPointerData = { this, &m_properties, &m_keys, &m_mouse };
		glfwSetWindowUserPointer(m_windowHandle, &m_windowUserPointerData);

		glfwSetWindowCloseCallback(m_windowHandle, [](GLFWwindow * windowHandle)
		{
			Window & window = *((WindowUserPointerData *)glfwGetWindowUserPointer(windowHandle))->window;
			window.getEvent<WindowCloseEvent>()->dispatch();
		});
		glfwSetWindowSizeCallback(m_windowHandle, [](GLFWwindow * windowHandle, int width, int height)
		{
			WindowUserPointerData & data = *(WindowUserPointerData *)glfwGetWindowUserPointer(windowHandle);
			data.properties->sizeWidth = width;
			data.properties->sizeHeight = height;

			Window & window = *((WindowUserPointerData *)glfwGetWindowUserPointer(windowHandle))->window;
			window.getEvent<WindowResizeEvent>()->dispatch(data.properties->sizeWidth, data.properties->sizeHeight);

		});
		glfwSetKeyCallback(m_windowHandle, [](GLFWwindow * windowHandle, int key, int scancode, int action, int mods)
		{
			std::unordered_map<Key, std::pair<KeyData, KeyData>> & keys = *((WindowUserPointerData *)glfwGetWindowUserPointer(windowHandle))->keys;

			auto setKeyData = [&](Key key, int action)
			{
				std::pair< KeyData, KeyData> & keyDataPair = keys[key];
				KeyData & oldKeyData = keyDataPair.second;
				KeyData & newKeyData = keyDataPair.first;

				newKeyData.isDown = action != GLFW_RELEASE;
				newKeyData.isRepeating = action == GLFW_REPEAT;
				newKeyData.justPressed = action == GLFW_PRESS && !oldKeyData.isDown;
				newKeyData.justReleased = action == GLFW_RELEASE && oldKeyData.isDown;

				oldKeyData = newKeyData;
			};

			switch (key)
			{
			case GLFW_KEY_ESCAPE: setKeyData(Key::Escape, action); break;
			case GLFW_KEY_GRAVE_ACCENT: setKeyData(Key::BackQuote, action); break;
			case GLFW_KEY_TAB: setKeyData(Key::Tab, action); break;
			case GLFW_KEY_CAPS_LOCK: setKeyData(Key::CapsLock, action); break;
			case GLFW_KEY_SCROLL_LOCK: setKeyData(Key::ScrollLock, action); break;
			case GLFW_KEY_NUM_LOCK: setKeyData(Key::NumLock, action); break;
			case GLFW_KEY_LEFT_SHIFT: setKeyData(Key::LeftShift, action); break;
			case GLFW_KEY_LEFT_CONTROL: setKeyData(Key::LeftCtrl, action); break;
			case GLFW_KEY_LEFT_ALT: setKeyData(Key::LeftAlt, action); break;
			case GLFW_KEY_SPACE: setKeyData(Key::Space, action); break;
			case GLFW_KEY_RIGHT_ALT: setKeyData(Key::RightAlt, action); break;
			case GLFW_KEY_RIGHT_CONTROL: setKeyData(Key::RightCtrl, action); break;
			case GLFW_KEY_RIGHT_SHIFT: setKeyData(Key::RightShift, action); break;
			case GLFW_KEY_ENTER: setKeyData(Key::Enter, action); break;
			case GLFW_KEY_BACKSPACE: setKeyData(Key::BackSpace, action); break;

			case GLFW_KEY_A: setKeyData(Key::A, action); break;
			case GLFW_KEY_B: setKeyData(Key::B, action); break;
			case GLFW_KEY_C: setKeyData(Key::C, action); break;
			case GLFW_KEY_D: setKeyData(Key::D, action); break;
			case GLFW_KEY_E: setKeyData(Key::E, action); break;
			case GLFW_KEY_F: setKeyData(Key::F, action); break;
			case GLFW_KEY_G: setKeyData(Key::G, action); break;
			case GLFW_KEY_H: setKeyData(Key::H, action); break;
			case GLFW_KEY_I: setKeyData(Key::I, action); break;
			case GLFW_KEY_J: setKeyData(Key::J, action); break;
			case GLFW_KEY_K: setKeyData(Key::K, action); break;
			case GLFW_KEY_L: setKeyData(Key::L, action); break;
			case GLFW_KEY_M: setKeyData(Key::M, action); break;
			case GLFW_KEY_N: setKeyData(Key::N, action); break;
			case GLFW_KEY_O: setKeyData(Key::O, action); break;
			case GLFW_KEY_P: setKeyData(Key::P, action); break;
			case GLFW_KEY_Q: setKeyData(Key::Q, action); break;
			case GLFW_KEY_R: setKeyData(Key::R, action); break;
			case GLFW_KEY_S: setKeyData(Key::S, action); break;
			case GLFW_KEY_T: setKeyData(Key::T, action); break;
			case GLFW_KEY_U: setKeyData(Key::U, action); break;
			case GLFW_KEY_V: setKeyData(Key::V, action); break;
			case GLFW_KEY_W: setKeyData(Key::W, action); break;
			case GLFW_KEY_X: setKeyData(Key::X, action); break;
			case GLFW_KEY_Y: setKeyData(Key::Y, action); break;
			case GLFW_KEY_Z: setKeyData(Key::Z, action); break;

			case GLFW_KEY_0: setKeyData(Key::D0, action); break;
			case GLFW_KEY_1: setKeyData(Key::D1, action); break;
			case GLFW_KEY_2: setKeyData(Key::D2, action); break;
			case GLFW_KEY_3: setKeyData(Key::D3, action); break;
			case GLFW_KEY_4: setKeyData(Key::D4, action); break;
			case GLFW_KEY_5: setKeyData(Key::D5, action); break;
			case GLFW_KEY_6: setKeyData(Key::D6, action); break;
			case GLFW_KEY_7: setKeyData(Key::D7, action); break;
			case GLFW_KEY_8: setKeyData(Key::D8, action); break;
			case GLFW_KEY_9: setKeyData(Key::D9, action); break;

			case GLFW_KEY_F1: setKeyData(Key::F1, action); break;
			case GLFW_KEY_F2: setKeyData(Key::F2, action); break;
			case GLFW_KEY_F3: setKeyData(Key::F3, action); break;
			case GLFW_KEY_F4: setKeyData(Key::F4, action); break;
			case GLFW_KEY_F5: setKeyData(Key::F5, action); break;
			case GLFW_KEY_F6: setKeyData(Key::F6, action); break;
			case GLFW_KEY_F7: setKeyData(Key::F7, action); break;
			case GLFW_KEY_F8: setKeyData(Key::F8, action); break;
			case GLFW_KEY_F9: setKeyData(Key::F9, action); break;
			case GLFW_KEY_F10: setKeyData(Key::F10, action); break;
			case GLFW_KEY_F11: setKeyData(Key::F11, action); break;
			case GLFW_KEY_F12: setKeyData(Key::F12, action); break;

			case GLFW_KEY_PRINT_SCREEN: setKeyData(Key::PrintScreen, action); break;
			case GLFW_KEY_PAUSE: setKeyData(Key::Pause, action); break;
			case GLFW_KEY_INSERT: setKeyData(Key::Insert, action); break;
			case GLFW_KEY_HOME: setKeyData(Key::Home, action); break;
			case GLFW_KEY_DELETE: setKeyData(Key::Delete, action); break;
			case GLFW_KEY_END: setKeyData(Key::End, action); break;
			case GLFW_KEY_PAGE_UP: setKeyData(Key::PageUp, action); break;
			case GLFW_KEY_PAGE_DOWN: setKeyData(Key::PageDown, action); break;

			case GLFW_KEY_LEFT: setKeyData(Key::Left, action); break;
			case GLFW_KEY_UP: setKeyData(Key::Up, action); break;
			case GLFW_KEY_DOWN: setKeyData(Key::Down, action); break;
			case GLFW_KEY_RIGHT: setKeyData(Key::Right, action); break;

			case GLFW_KEY_KP_DIVIDE: setKeyData(Key::NumPad_Divide, action); break;
			case GLFW_KEY_KP_MULTIPLY: setKeyData(Key::NumPad_Multiply, action); break;
			case GLFW_KEY_KP_SUBTRACT: setKeyData(Key::NumPad_Subtract, action); break;
			case GLFW_KEY_KP_ADD: setKeyData(Key::NumPad_Plus, action); break;
			case GLFW_KEY_KP_ENTER: setKeyData(Key::NumPad_Plus, action); break;
			case GLFW_KEY_KP_DECIMAL: setKeyData(Key::NumPad_Delete, action); break;
			case GLFW_KEY_KP_0: setKeyData(Key::NumPad_0, action); break;
			case GLFW_KEY_KP_1: setKeyData(Key::NumPad_1, action); break;
			case GLFW_KEY_KP_2: setKeyData(Key::NumPad_2, action); break;
			case GLFW_KEY_KP_3: setKeyData(Key::NumPad_3, action); break;
			case GLFW_KEY_KP_4: setKeyData(Key::NumPad_4, action); break;
			case GLFW_KEY_KP_5: setKeyData(Key::NumPad_5, action); break;
			case GLFW_KEY_KP_6: setKeyData(Key::NumPad_6, action); break;
			case GLFW_KEY_KP_7: setKeyData(Key::NumPad_7, action); break;
			case GLFW_KEY_KP_8: setKeyData(Key::NumPad_8, action); break;
			case GLFW_KEY_KP_9: setKeyData(Key::NumPad_9, action); break;
			}
		});
		glfwSetCharCallback(m_windowHandle, [](GLFWwindow * windowHandle, unsigned int charCode)
		{
			Window & window = *((WindowUserPointerData *)glfwGetWindowUserPointer(windowHandle))->window;
			window.getEvent<CharInputEvent>()->dispatch(charCode);
		});
		glfwSetCursorPosCallback(m_windowHandle, [](GLFWwindow* windowHandle, double xPos, double yPos)
		{
			MouseData & mouse = *((WindowUserPointerData *)glfwGetWindowUserPointer(windowHandle))->mouse;
			mouse.delta.x = xPos - mouse.position.x;
			mouse.delta.y = yPos - mouse.position.y;
			mouse.position.x = xPos;
			mouse.position.y = yPos;
		});
		glfwSetMouseButtonCallback(m_windowHandle, [](GLFWwindow * windowHandle, int button, int action, int mods)
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
		glfwSetScrollCallback(m_windowHandle, [](GLFWwindow* windowHandle, double xOffset, double yOffset)
		{
			MouseData& mouse = *((WindowUserPointerData*)glfwGetWindowUserPointer(windowHandle))->mouse;
			mouse.scrollDelta.x = xOffset;
			mouse.scrollDelta.y = yOffset;
		});
	}

	void Window::shutdown()
	{
		glfwDestroyWindow(m_windowHandle);
	}
}

alvere::Asset<alvere::Window> alvere::Window::New(const alvere::Window::Properties& properties)
{
	return alvere::Asset<alvere::platform::windows::Window>(new alvere::platform::windows::Window(properties));
}