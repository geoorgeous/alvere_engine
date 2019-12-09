#pragma once

#include <vector>
#include <memory>

class Command;

struct CommandStack
{
	std::vector<std::unique_ptr<Command>> m_commandHistory;
	int m_currentOffset;

	CommandStack();
	~CommandStack();

	void Add(Command * c);

	void Undo();
	void Redo();
};