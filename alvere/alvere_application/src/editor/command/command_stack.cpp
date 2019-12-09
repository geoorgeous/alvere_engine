#include "editor/command/command_stack.hpp"
#include "editor/command/command.hpp"

CommandStack::CommandStack()
	: m_currentOffset(0)
{
}

CommandStack::~CommandStack()
{
	//This must be defined or else unique ptr will moan...
}

void CommandStack::Add(Command * command)
{
	//Remove any commands that have been undone as this new command will invalidate them
	for (; m_currentOffset > 0; --m_currentOffset)
	{
		m_commandHistory.pop_back();
	}

	m_commandHistory.emplace_back(command);
	m_commandHistory.back()->Execute();
}

void CommandStack::Undo()
{
	if (m_currentOffset == m_commandHistory.size())
	{
		//Cannot undo if we have nothing active left in the stack
		return;
	}

	m_currentOffset += 1;
	m_commandHistory[m_commandHistory.size() - m_currentOffset]->Undo();
}

void CommandStack::Redo()
{
	if (m_currentOffset == 0)
	{
		//Cannot redo if we have nothing inactive left in the stack
		return;
	}

	m_commandHistory[m_commandHistory.size() - m_currentOffset]->Execute();
	m_currentOffset -= 1;
}