#include <GL/glfw3.h>

#include <iostream>

#include "Engine/Engine.h"
#include "Timer/FrameTime.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"


int main(void)
{
	Engine::Startup();
	Engine::Input::Startup();

	GLFWwindow* window;
	const char* WindowName = "Shantanu's Window: Hello World!";
	const int WindowWidth = 800;
	const int WindowHeight = 600;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WindowWidth, WindowHeight, WindowName, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	Engine::Renderer::Startup();
	Engine::Timing::CalcCPUFrequency();
	float dt;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		 dt = Engine::Timing::GetLastFrameTime_ms();

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		
		Engine::Input::Update(window, dt);
		Engine::Renderer::Update(window, dt);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	Engine::Renderer::Shutdown();
	glfwTerminate();

	Engine::Input::Shutdown();
	Engine::Shutdown();
    
	return 0;
}



/*References:
https://www.glfw.org/documentation.html for GLFW startup, main loop, termination and window creation
*/
