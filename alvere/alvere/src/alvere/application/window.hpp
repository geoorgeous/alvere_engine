#pragma once

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include "alvere/events/application_events.hpp"
#include "alvere/graphics/rendering_context.hpp"
#include "alvere/graphics/texture.hpp"
#include "alvere/math/vector/vec_2_i.hpp"

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

	enum KeyModifier
	{
		Shift		= 1u << 0,
		Ctrl		= 1u << 1,
		Alt			= 1u << 2,
		Super		= 1u << 3,
		CapsLock	= 1u << 4,
		NumLock		= 1u << 5,
	};

	enum class MouseButton
	{
		Left,
		Right,
		Middle,
		Button1 = Left,
		Button2 = Right,
		Button3 = Middle,
		Button4,
		Button5,
		Button6,
		Button7,
		Button8,
	};

	struct KeyData
	{
		bool isDown;
		bool isRepeating;
		bool justPressed;
		bool justReleased;
		uint8_t modifiers;

		inline operator bool() const
		{
			return isDown;
		}
	};

	struct MouseButtonData
	{
		bool isDown;
		bool justPressed;
		bool justReleased;
		uint8_t modifiers;

		inline operator bool() const
		{
			return isDown;
		}
	};

	struct MouseData
	{
		Vector2 position;
		Vector2 delta;
		Vector2 scrollDelta;
		std::unordered_map<MouseButton, std::pair<MouseButtonData, MouseButtonData>> buttons;

		MouseButtonData getButton(MouseButton button) const;
	};

	enum class CursorType
	{
		Default,
		Arrow,
		IBeam,
		Crosshair,
		Hand,
		ResizeHorizontal,
		ResizeVertical
	};

	class Window
	{
	public:

		struct Properties
		{
			static const Properties s_default;

			std::string m_title;
			Vec2i m_size;
			std::int8_t m_flags;

			Properties(const std::string & title);
			Properties(const std::string & title, Vec2i size);
			Properties(const std::string & title, Vec2i size, unsigned char m_flags);
		};

		enum Flag : std::uint8_t
		{
			IsVisible				= 1u << 0,
			Resizeable				= 1u << 1,
			AlwaysOnTop				= 1u << 2,
			Decorated				= 1u << 3,
			FullScreen				= 1u << 4,
			FullScreenAutoMinimize	= 1u << 5,
			IsCursorEnabled			= 1u << 6
		};

		static std::unique_ptr<Window> create(const Properties & properties = Window::Properties::s_default);

		static std::unique_ptr<Window> create(const Properties & properties, Vec2i resolution);

		virtual ~Window() = 0;

		virtual void setTitle(std::string title) = 0;

		virtual void setPosition(int x, int y) = 0;

		virtual void resize(int width, int height) = 0;

		virtual void setIcon(const std::vector<const Texture> & iconTextures) = 0;

		virtual void setSizeLimits(int minWidth, int minHeight, int maxWidth, int maxHeight) = 0;

		virtual void setCursor(CursorType cursorType) = 0;

		virtual void maximize() = 0;

		virtual void minimize() = 0;

		virtual void restore() = 0;

		virtual void focus() = 0;

		virtual void requestAttention() = 0;

		virtual void pollEvents() = 0;

		virtual void swapBuffers() = 0;

		inline std::string getTitle() const
		{
			return m_title;
		}

		inline Vec2i getPosition() const
		{
			return m_position;
		}

		inline Vec2i getSize() const
		{
			return m_size;
		}

		inline bool getFlag(Flag flag) const
		{
			return (m_flags & flag) == flag;
		}

		inline void setFlags(std::uint8_t flags)
		{
			m_flags = flags;
		}

		inline virtual void setFlag(Flag flag, bool value)
		{
			if(value) m_flags |= flag;
			else m_flags &= ~flag;
		}

		inline float getAspectRatio() const
		{
			return (float)m_size.y / (float)m_size.x;
		}

		inline const RenderingContext & getRenderingContext() const
		{
			return *m_renderingContext;
		}

		KeyData getKey(Key key) const;

		const MouseData& getMouse() const;

		template <typename EventT>
		EventT * getEvent()
		{
			return (EventT *)m_events[typeid(EventT)];
		}

	protected:

		std::string m_title;

		Vec2i m_position;

		Vec2i m_size;

		std::uint8_t m_flags;

		alvere::RenderingContext * m_renderingContext;

		std::unordered_map<Key, std::pair<KeyData, KeyData>> m_keys;

		MouseData m_mouse;

		std::unordered_map<std::type_index, EventBase *> m_events;

		Window();
	};
}