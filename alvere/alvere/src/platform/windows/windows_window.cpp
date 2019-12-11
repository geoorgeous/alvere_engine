#include <glfw/glfw3.h>

#include "platform/windows/windows_window.hpp"

#include "alvere/debug/exceptions.hpp"
#include "alvere/events/application_events.hpp"
#include "alvere/graphics/render_commands.hpp"
#include "alvere/math/vector/vec_2_i.hpp"
#include "graphics_api/opengl/opengl_context.hpp"

namespace alvere::platform::windows
{
	static bool s_GLFWInitialised = false;

	Window::Window(const alvere::Window::Properties & properties, Vec2i resolution)
		: alvere::Window()
	{
		init(properties, resolution);
	}

	Window::~Window()
	{
		shutdown();
	}

	void Window::setTitle(std::string title)
	{
		m_title = title;
		glfwSetWindowTitle(m_windowHandle, title.c_str());
	}

	void Window::setPosition(int x, int y)
	{
		glfwSetWindowPos(m_windowHandle, x, y);
	}

	void Window::resize(int width, int height)
	{
		glfwSetWindowSize(m_windowHandle, width, height);
	}

	void Window::setFlag(Flag flag, bool value)
	{
		if(getFlag(flag) == value)
			return;
		alvere::Window::setFlag(flag, value);

		switch (flag)
		{
			case Flag::IsVisible:
				if (value)
					glfwShowWindow(m_windowHandle);
				else
					glfwHideWindow(m_windowHandle);
				break;

			case Flag::Resizeable:
				glfwSetWindowAttrib(m_windowHandle, GLFW_RESIZABLE, value ? GLFW_TRUE : GLFW_FALSE);
				break;

			case Flag::AlwaysOnTop:
				glfwSetWindowAttrib(m_windowHandle, GLFW_FLOATING, value ? GLFW_TRUE : GLFW_FALSE);
				break;

			case Flag::Decorated:
				glfwSetWindowAttrib(m_windowHandle, GLFW_DECORATED, value ? GLFW_TRUE : GLFW_FALSE);
				break;

			case Flag::FullScreen:
				if (value)
				{
					m_windowedModePosition = m_position;
					m_windowedModeSize = m_size;
					const GLFWvidmode * mode = glfwGetVideoMode(m_monitorHandle);
					glfwSetWindowMonitor(m_windowHandle, m_monitorHandle, 0, 0, mode->width, mode->height, mode->refreshRate);
				}
				else
				{
					glfwSetWindowMonitor(m_windowHandle, NULL, m_windowedModePosition.x, m_windowedModePosition.y, m_windowedModeSize.x, m_windowedModeSize.y, 0);
				}
				break;

			case Flag::FullScreenAutoMinimize:
				glfwSetWindowAttrib(m_windowHandle, GLFW_AUTO_ICONIFY, value ? GLFW_TRUE : GLFW_FALSE);
				break;

			case Flag::IsCursorEnabled:
				if (value)
					glfwSetInputMode(m_windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				else
					glfwSetInputMode(m_windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				break;
		}
	}

	void Window::setIcon(const std::vector<const Texture> & iconTextures)
	{
		// todo
		//GLFWimage * images = new GLFWimage[iconTextures.size];
		//glfwSetWindowIcon(m_windowHandle, iconTextures.size, images);
	}

	void Window::setSizeLimits(int minWidth, int minHeight, int maxWidth, int maxHeight)
	{
		glfwSetWindowSizeLimits(m_windowHandle, minWidth, minHeight, maxWidth, maxHeight);
	}

	void Window::setCursor(CursorType cursorType)
	{
		glfwDestroyCursor(m_cursor);

		switch(cursorType)
		{
			case CursorType::Arrow: m_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR); break;
			case CursorType::IBeam: m_cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR); break;
			case CursorType::Crosshair: m_cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR); break;
			case CursorType::Hand: m_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR); break;
			case CursorType::ResizeHorizontal: m_cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR); break;
			case CursorType::ResizeVertical: m_cursor = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR); break;
		}

		glfwSetCursor(m_windowHandle, m_cursor);
	}

	void Window::maximize()
	{
		glfwMaximizeWindow(m_windowHandle);
	}

	void Window::minimize()
	{
		glfwIconifyWindow(m_windowHandle);
	}

	void Window::restore()
	{
		glfwRestoreWindow(m_windowHandle);
	}

	void Window::focus()
	{
		glfwFocusWindow(m_windowHandle);
	}

	void Window::requestAttention()
	{
		glfwRequestWindowAttention(m_windowHandle);
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

	void Window::swapBuffers()
	{
		render_commands::setViewport(0, 0, m_size.x, m_size.y);

		m_renderingContext->renderFrameBuffer();

		glfwSwapBuffers(m_windowHandle);
	}

	void Window::init(const alvere::Window::Properties& properties, Vec2i resolution)
	{
		if (!s_GLFWInitialised)
		{
			if (!glfwInit())
			{
				AlvThrowFatal("Failed to initialise GLFW.\n");
			}

			glfwSetErrorCallback([](int error, const char * message)
			{
				AlvThrow("GLFW error (%i): Message: \"%s\".\n", error, message);
			});

			s_GLFWInitialised = true;
		}

		setFlags(properties.m_flags);

		glfwWindowHint(GLFW_VISIBLE,  getFlag(Flag::IsVisible));
		glfwWindowHint(GLFW_RESIZABLE, getFlag(Flag::Resizeable));
		glfwWindowHint(GLFW_FLOATING, getFlag(Flag::AlwaysOnTop));
		glfwWindowHint(GLFW_DECORATED, getFlag(Flag::Decorated));
		glfwWindowHint(GLFW_AUTO_ICONIFY, getFlag(Flag::FullScreenAutoMinimize));

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		if (getFlag(Flag::FullScreen))
		{
			GLFWmonitor * monitor = glfwGetPrimaryMonitor();

			const GLFWvidmode * videoMode = glfwGetVideoMode(monitor);
			resolution.x = (float)resolution.x * (float)videoMode->width / resolution.x;
			resolution.y = (float)resolution.y * (float)videoMode->height / resolution.y;

			m_windowedModeSize = properties.m_size;
			m_size.x = videoMode->width;
			m_size.y = videoMode->height;

			m_windowHandle = glfwCreateWindow(m_size.x, m_size.y, properties.m_title.c_str(), monitor, nullptr);
		}
		else
		{
			m_size = properties.m_size;
			m_windowedModeSize = m_size;

			m_windowHandle = glfwCreateWindow(m_size.x, m_size.y, properties.m_title.c_str(), nullptr, nullptr);
		}

		if (m_windowHandle == NULL)
		{
			AlvThrowFatal("Failed to create GLFW window.\n");
		}

		setFlag(Flag::IsCursorEnabled, (properties.m_flags & Flag::IsCursorEnabled) == Flag::IsCursorEnabled);

		glfwMakeContextCurrent(m_windowHandle);

		m_renderingContext = new alvere::graphics_api::opengl::RenderingContext;
		m_renderingContext->init(resolution.x, resolution.y);

		m_title = properties.m_title;
		glfwGetWindowPos(m_windowHandle, &m_position.x, &m_position.y);
		m_windowedModePosition = m_position;

		m_windowUserPointerData = { this, &m_position, &m_keys, &m_mouse, &Window::resizeCallback };
		glfwSetWindowUserPointer(m_windowHandle, &m_windowUserPointerData);

		m_monitorHandle = glfwGetPrimaryMonitor();
		
		glfwSwapInterval(0);

		setCallbacks();
	}

	void Window::setCallbacks()
	{
		glfwSetWindowCloseCallback(m_windowHandle, [](GLFWwindow * windowHandle)
		{
			Window & window = *((WindowUserPointerData *)glfwGetWindowUserPointer(windowHandle))->window;
			window.getEvent<WindowCloseEvent>()->dispatch();
		});

		glfwSetWindowPosCallback(m_windowHandle, [](GLFWwindow * windowHandle, int x, int y)
		{
			Vec2i & position = *((WindowUserPointerData *)glfwGetWindowUserPointer(windowHandle))->position;
			position.x = x;
			position.y = y;
		});

		glfwSetWindowSizeCallback(m_windowHandle, [](GLFWwindow * windowHandle, int width, int height)
		{
			WindowUserPointerData * windowUserPointerData = (WindowUserPointerData *)glfwGetWindowUserPointer(windowHandle);
			(windowUserPointerData->window->*(windowUserPointerData->windowResizeCallback))(width, height);
		});

		glfwSetFramebufferSizeCallback(m_windowHandle, [](GLFWwindow * windowHandle, int width, int height)
		{
			WindowUserPointerData * windowUserPointerData = (WindowUserPointerData *)glfwGetWindowUserPointer(windowHandle);
			(windowUserPointerData->window->*(windowUserPointerData->windowResizeCallback))(width, height);
		});

		glfwSetWindowFocusCallback(m_windowHandle, [](GLFWwindow * windowHandle, int focus)
		{
			Window & window = *((WindowUserPointerData *)glfwGetWindowUserPointer(windowHandle))->window;
			window.getEvent<WindowFocusEvent>()->dispatch(focus == GLFW_TRUE ? true : false);
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

		glfwSetCursorPosCallback(m_windowHandle, [](GLFWwindow * windowHandle, double xPos, double yPos)
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
				case GLFW_MOUSE_BUTTON_4: mouse.buttons[MouseButton::Button4] = isDown; break;
				case GLFW_MOUSE_BUTTON_5: mouse.buttons[MouseButton::Button5] = isDown; break;
				case GLFW_MOUSE_BUTTON_6: mouse.buttons[MouseButton::Button6] = isDown; break;
				case GLFW_MOUSE_BUTTON_7: mouse.buttons[MouseButton::Button7] = isDown; break;
				case GLFW_MOUSE_BUTTON_8: mouse.buttons[MouseButton::Button8] = isDown; break;
			}
		});

		glfwSetScrollCallback(m_windowHandle, [](GLFWwindow * windowHandle, double xOffset, double yOffset)
		{
			MouseData & mouse = *((WindowUserPointerData *)glfwGetWindowUserPointer(windowHandle))->mouse;
			mouse.scrollDelta.x = xOffset;
			mouse.scrollDelta.y = yOffset;
		});
	}

	void Window::shutdown()
	{
		glfwDestroyWindow(m_windowHandle);
	}

	void Window::resizeCallback(int width, int height)
	{
		float resolutionScaleX = m_size.x == 0 ? 0 : m_renderingContext->getResolution().x / m_size.x;
		float resolutionScaleY = m_size.y == 0 ? 0 : m_renderingContext->getResolution().y / m_size.y;

		m_size.x = width;
		m_size.y = height;

		m_renderingContext->setResolution(resolutionScaleX * m_size.x, resolutionScaleY * m_size.y);
		alvere::render_commands::setViewport(0, 0, m_size.x, m_size.y);

		getEvent<WindowResizeEvent>()->dispatch(m_size.x, m_size.y);
	}
}

std::unique_ptr<alvere::Window> alvere::Window::create(const alvere::Window::Properties & properties)
{
	return std::make_unique<alvere::platform::windows::Window>(properties, properties.m_size);
}

std::unique_ptr<alvere::Window> alvere::Window::create(const alvere::Window::Properties & properties, alvere::Vec2i resolution)
{
	return std::make_unique<alvere::platform::windows::Window>(properties, resolution);
}