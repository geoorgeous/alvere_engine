#pragma once

#include <stdexcept>
#include <iostream>
#include <string>

#include "alvere/application/application.hpp"
#include "alvere/debug/exceptions.hpp"
#include "alvere/debug/logging.hpp"

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
		alvere::LogError("Initialisation error! %s\n", e.what());
		error = 1;

		{ //Wait for user input before closing the terminal
			std::cout << "\nPress any key to exit ...";
			std::getchar();
		}
	}

	delete app;

	return error;
}