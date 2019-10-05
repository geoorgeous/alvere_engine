#include "alvere/utils/command_console/command_alias.hpp"

#include "alvere/utils/command_console/arg.hpp"
#include "alvere/utils/command_console/command_console.hpp"

namespace alvere::console
{
	CommandAlias::CommandAlias(const char * name, const char * description, const char * command)
		: m_command(command), Command(name, description,
			[&](std::vector<const IArg *> args) -> std::string
	{
		return m_command + "\n" + submitCommand(m_command);
	})
	{ }
}