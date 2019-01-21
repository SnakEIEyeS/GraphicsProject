#include "Engine.h"

#include <iostream>

namespace Engine
{
	bool Startup()
	{
		std::cout << "Engine Startup\n";
		return true;
	}

	bool Shutdown()
	{
		std::cout << "Engine Shutdown\n";
		return true;
	}
}