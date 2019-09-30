#pragma once

#include <string>
#include <unordered_map>

#include "alvere/assets.hpp"
#include "alvere/events/application_events.hpp"
#include "alvere/math/vectors.hpp"

namespace alvere
{
	class FrameBuffer;

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
		std::unordered_map<MouseButton, bool> buttons;

		bool GetButton(MouseButton button) const;
	};

	class Window
	{
	public:
		using EventCallbackFunction = std::function<void(Event&)>;

		struct Properties
		{
			static const Properties s_Default;

			std::string title;
			unsigned int sizeWidth;
			unsigned int sizeHeight;
			unsigned int resWidth;
			unsigned int resHeight;

			Properties(const std::string& title);
			Properties(const std::string& title, unsigned int sizeWidth, unsigned int sizeHeight);
			Properties(const std::string& title, unsigned int sizeWidth, unsigned int sizeHeight, unsigned int resWidth, unsigned int resHeight);
		};

		static Asset<Window> New(const Properties& properties = Window::Properties::s_Default);

		virtual ~Window() { }

		virtual void PollEvents() = 0;
		virtual void SwapBuffers() = 0;
		virtual void DisableCursor() = 0;
		virtual void EnableCursor() = 0;
		virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;

		KeyData GetKey(Key key) const;
		const MouseData& GetMouse() const;

	protected:
		EventCallbackFunction m_EventCallback;
		FrameBuffer * m_FrameBuffer;
		std::unordered_map<Key, KeyData> m_Keys;
		MouseData m_Mouse;
	};
}