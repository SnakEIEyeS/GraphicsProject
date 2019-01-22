#pragma once

struct GLFWwindow;

namespace Engine
{
	namespace Input
	{
		bool Startup();
		bool Shutdown();

		void Update(GLFWwindow* window, float i_FrameTime);
		void ProcessInput(GLFWwindow * window);
	} //namespace Input
} //namespace Engine
