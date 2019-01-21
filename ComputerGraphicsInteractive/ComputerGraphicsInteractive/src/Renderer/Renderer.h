#pragma once

#include <GL/glfw3.h>
//struct GLFWwindow;

#include "../Math/Vector3.h"

namespace Engine
{
	namespace Renderer
	{
		struct Color
		{
			GLclampf red, green, blue, alpha;

			Color() : red(0.f), green(0.f), blue(0.f), alpha(0.f)
			{}

			Color(GLclampf i_Red, GLclampf i_Green, GLclampf i_Blue, GLclampf i_Alpha) : red(i_Red), green(i_Green), blue(i_Blue), alpha(i_Alpha)
			{}
		};

		

		bool Startup();
		bool Shutdown();

		void Update(GLFWwindow* window);
		void AnimateClearColor();
	}
}