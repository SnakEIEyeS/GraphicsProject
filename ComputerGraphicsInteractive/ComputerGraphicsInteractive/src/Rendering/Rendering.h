#pragma once

#include <glad/glad.h>
#include <GL/glfw3.h>
//struct GLFWwindow;

#include "../cyCodeBase/cyGL.h"
#include "../Math/Vector3.h"

namespace Engine
{
	namespace Rendering
	{
		static cyGLSLProgram* cyGraphicsProgram = nullptr;
		
		struct Color
		{
			float red, green, blue, alpha;

			Color() : red(0.f), green(0.f), blue(0.f), alpha(0.f)
			{}

			Color(float i_Red, float i_Green, float i_Blue, float i_Alpha) : red(i_Red), green(i_Green), blue(i_Blue), alpha(i_Alpha)
			{}
		};

		

		bool Startup();
		bool Shutdown();

		void Update(GLFWwindow* window, float i_FrameTime);
		void AnimateClearColor(float i_FrameTime);

		cyGLSLProgram* GetGLProgram();
		bool BuildAndUseProgram();

		
	}
}