#pragma once

#include "input_button.hpp"

namespace alvere
{
	class KeyButton : public InputButton
	{
		Key m_key;
		bool m_oldState;
		bool m_newState;

	public:

		KeyButton(Key key);

		void Update(Window & window) override;

		bool IsDown() const override;

		bool IsPressed() const override;
		bool IsReleased() const override;
	};
}