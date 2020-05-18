#pragma once

#include <string>

namespace alvere
{
	class Component
	{
	public:
		virtual ~Component() {}
		virtual std::string to_string() const { return ""; }
	};
}