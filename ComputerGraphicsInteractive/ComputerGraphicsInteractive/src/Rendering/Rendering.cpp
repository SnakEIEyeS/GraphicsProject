#include "Rendering.h"

#include <assert.h>
#include <iostream>

#include "../cyCodeBase/cyGL.h"
#include "../cyCodeBase/cyPoint.h"
#include "../Math/MathUtility.h"
#include "../LightSource/PointLight.h"
#include "Renderer.h"

namespace Engine
{
	namespace Rendering
	{
		static const char* VertexShaderFile = "res/LightsShadingVS.shader";
		static const char* FragmentShaderFile = "res/LightsShadingFS.shader";

		static const float ColorDelta = 0.00025f;
		//const float ColorDeltaEpsilon = 0.0001f;
		static Engine::Math::Vector3 ClearColorDeltaVec(0.f, 0.f, ColorDelta);
		static Color ClearColor(0.f, 0.f, 0.f, 1.f);

		static const cyPoint4f PointLightCreatePosition(cyPoint4f(0.f, -10.f, 40.f, 1.f));

		bool Startup()
		{
			std::cout << "Renderer Startup\n";
			return true;
		}

		bool Shutdown()
		{
			std::cout << "Renderer Shutdown\n";
			return true;
		}

		void Update(GLFWwindow* window, float i_FrameTime)
		{
			//AnimateClearColor(i_FrameTime);

			if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
			{
				std::cout << "Recompiling shaders...\n";
				Engine::Rendering::BuildAndUseProgram();
			}
		}

		void AnimateClearColor(float i_FrameTime)
		{
			glClearColor(ClearColor.red, ClearColor.green, ClearColor.blue, ClearColor.alpha);
			
			ClearColor.red += ClearColorDeltaVec.x * i_FrameTime;
			/*if (Engine::Math::AreAboutEqual(ClearColor.red, 1.f, ColorDeltaEpsilon))
			{
				ClearColorDeltaVec.x = -ColorDelta;
				ClearColorDeltaVec.y = ColorDelta;
			}
			if (Engine::Math::AreAboutEqual(ClearColor.red, 0.f, ColorDeltaEpsilon))
			{
				ClearColorDeltaVec.x = 0.f;
			}

			ClearColor.green += ClearColorDeltaVec.y;
			if (Engine::Math::AreAboutEqual(ClearColor.green, 1.f, ColorDeltaEpsilon))
			{
				ClearColorDeltaVec.y = -ColorDelta;
				ClearColorDeltaVec.z = ColorDelta;
			}
			if (Engine::Math::AreAboutEqual(ClearColor.green, 0.f, ColorDeltaEpsilon))
			{
				ClearColorDeltaVec.y = 0.f;
			}

			ClearColor.blue += ClearColorDeltaVec.z;
			if (Engine::Math::AreAboutEqual(ClearColor.blue, 1.f, ColorDeltaEpsilon))
			{
				ClearColorDeltaVec.z = -ColorDelta;
				ClearColorDeltaVec.x = ColorDelta;
			}
			if (Engine::Math::AreAboutEqual(ClearColor.blue, 0.f, ColorDeltaEpsilon))
			{
				ClearColorDeltaVec.z = 0.f;
			}*/

			if (ClearColor.red >= 1.f || ClearColor.red <= 0.f)
			{
				ClearColorDeltaVec.x = -ClearColorDeltaVec.x;
			}

			ClearColor.green += ClearColorDeltaVec.y * i_FrameTime;
			if (ClearColor.green >= 1.f || ClearColor.green <= 0.f)
			{
				ClearColorDeltaVec.y = -ClearColorDeltaVec.y;
			}

			ClearColor.blue += ClearColorDeltaVec.z * i_FrameTime;
			if(ClearColor.blue >= 1.f || ClearColor.blue <= 0.f)
			{
				ClearColorDeltaVec.z = -ClearColorDeltaVec.z;
			}
		}

		cyGLSLProgram * GetGLProgram()
		{
			if (!cyGraphicsProgram)
			{
				cyGraphicsProgram = new cyGLSLProgram();
			}
			return cyGraphicsProgram;
		}

		bool BuildAndUseProgram()
		{
			bool built = GetGLProgram()->BuildFiles(VertexShaderFile, FragmentShaderFile, nullptr, nullptr, nullptr);
			assert(built);
			GetGLProgram()->Bind();
			return true;
		}


		Engine::Lighting::PointLight& GetRenderPointLight()
		{
			if (!RenderPointLight)
			{
				RenderPointLight = new Engine::Lighting::PointLight(PointLightCreatePosition);
			}

			return *RenderPointLight;
		}

	}
}