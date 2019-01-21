#include "Input.h"

#include <assert.h>
#include <iostream>

#include <GL/glfw3.h>

namespace Engine
{
	namespace Input
	{
		bool Startup()
		{
			std::cout << "Input Startup\n";
			return true;
		}

		bool Shutdown()
		{
			std::cout << "Input Shutdown\n";
			return true;
		}


		void Update(GLFWwindow * window)
		{
			assert(window);
			ProcessInput(window);
		}

		void ProcessInput(GLFWwindow * window)
		{
			assert(window);

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(window, true);
			}
		}

	}
}
