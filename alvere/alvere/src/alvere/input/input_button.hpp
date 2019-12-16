#pragma once

#include "alvere/application/window.hpp"

namespace alvere::input
{
	class InputButton
	{
	public:

		virtual ~InputButton() = default;

		virtual void update() = 0;

		virtual bool isDown() const = 0;

		virtual bool isPressed() const = 0;

		virtual bool isReleased() const = 0;
	};
}