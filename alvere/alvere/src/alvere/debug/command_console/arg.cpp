#include "alvere/debug/command_console/arg.hpp"

namespace alvere::console
{
	IArg::IArg(std::type_index typeIndex)
		: m_typeIndex(typeIndex)
	{ }
}