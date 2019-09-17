#pragma once

#include <stdexcept>
#include <iostream>

#include "alvere/application/application.hpp"
#include "alvere/utils/exceptions.hpp"
#include "alvere/utils/logging.hpp"

int main(int argc, char *argv[])
{
	alvere::Application * app = nullptr;
	int error = 0;

	try
	{
		app = alvere::NewApplication();
		app->run();
	}
	catch (alvere::Exception e)
	{
		alvere::LogError("Failed to initialise application: %s\n", e.what());
		error = 1;
	}

	delete app;

	return error;
}