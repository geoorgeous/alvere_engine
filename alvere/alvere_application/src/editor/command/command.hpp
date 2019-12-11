#pragma once

#include <string>

class Command
{
public:

	virtual ~Command() = default;

	virtual void Undo() = 0;
	virtual void Redo() = 0;

	virtual std::string GetDescription() = 0;
};