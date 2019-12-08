#pragma once

#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include "alvere/assets.hpp"
#include "alvere/graphics/frame_buffer.hpp"
#include "alvere/graphics/rendering_context.hpp"
#include "alvere/events/application_events.hpp"
#include "alvere/math/vectors.hpp"

namespace alvere
{
	enum class Key
	{
		Escape,
		BackQuote,
		Tab,
		CapsLock,
		NumLock,
		ScrollLock,
		LeftShift,
		LeftCtrl,
		LeftAlt,
		Space,
		RightAlt,
		RightCtrl,
		RightShift,
		Enter,
		BackSpace,

		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		D0,
		D1,
		D2,
		D3,
		D4,
		D5,
		D6,
		D7,
		D8,
		D9,

		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		
		PrintScreen,
		Pause,
		Insert,
		Home,
		Delete,
		End,
		PageUp,
		PageDown,

		Left,
		Up,
		Down,
		Right,

		NumPad_Divide,
		NumPad_Multiply,
		NumPad_Subtract,
		NumPad_Plus,
		NumPad_Enter,
		NumPad_Delete,
		NumPad_0,
		NumPad_1,
		NumPad_2,
		NumPad_3,
		NumPad_4,
		NumPad_5,
		NumPad_6,
		NumPad_7,
		NumPad_8,
		NumPad_9,
	};

	struct KeyData
	{
		bool isDown;
		bool isRepeating;
		bool justPressed;
		bool justReleased;

		inline operator bool() const
		{
			return isDown;
		}
	};

	enum class MouseButton
	{
		Left,
		Right,
		Middle,
	};

	struct MouseData
	{
		Vector2 position;
		Vector2 delta;
		Vector2 scrollDelta;
		std::unordered_map<MouseButton, bool> buttons;

		bool getButton(MouseButton button) const;
	};

	class Window
	{
	public:

		struct Properties
		{
			static const Properties s_default;

			std::string title;
			unsigned int sizeWidth;
			unsigned int sizeHeight;
			unsigned int resWidth;
			unsigned int resHeight;

			Properties(const std::string & title);
			Properties(const std::string & title, unsigned int sizeWidth, unsigned int sizeHeight);
			Properties(const std::string & title, unsigned int sizeWidth, unsigned int sizeHeight, unsigned int resWidth, unsigned int resHeight);
		};

		static Asset<Window> New(const Properties & properties = Window::Properties::s_default);

		virtual ~Window() = 0;

		virtual void pollEvents() = 0;

		virtual void disableCursor() = 0;

		virtual void enableCursor() = 0;

		void swapBuffers();

		inline unsigned int getWidth() const
		{
			return m_properties.sizeWidth;
		}

		inline unsigned int getHeight() const
		{
			return m_properties.sizeHeight;
		}

		inline RenderingContext & getRenderingContext()
		{
			return *m_renderingContext;
		}

		template <typename EventT>
		EventT * getEvent()
		{
			return (EventT *)m_events[typeid(EventT)];
		}

		KeyData getKey(Key key) const;

		const MouseData& getMouse() const;

	protected:

		Properties m_properties;

		alvere::RenderingContext * m_renderingContext;

		std::unordered_map<Key, std::pair<KeyData, KeyData>> m_keys;

		MouseData m_mouse;

		std::unordered_map<std::type_index, EventBase *> m_events;

		Window(const Properties & properties);
	};
}