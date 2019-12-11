#pragma once

#include "alvere/debug/command_console/command.hpp"

namespace alvere::console
{
	class CommandAlias : public Command
	{
	public:

		CommandAlias(const char * name, const char * description, const char * command);

		inline const std::string & getCommandString() const
		{
			return m_command;
		}

	private:

		std::string m_command;
	};
}