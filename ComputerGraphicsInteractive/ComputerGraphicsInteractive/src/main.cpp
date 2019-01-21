#include <GL/glfw3.h>

#include <iostream>

#include "Engine/Engine.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"


int main(void)
{
	Engine::Startup();
	Engine::Input::Startup();

	GLFWwindow* window;
	const char* WindowName = "Shantanu's Window: Hello World";
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

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		//glClearColor(0.f, 0.f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Engine::Input::Update(window);
		Engine::Renderer::Update(window);

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