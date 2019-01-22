#include "Renderer.h"

#include <iostream>

#include "../Math/MathUtility.h"

namespace Engine
{
	namespace Renderer
	{
		static const float ColorDelta = 0.00025f;
		//const float ColorDeltaEpsilon = 0.0001f;
		static Engine::Math::Vector3 ClearColorDeltaVec(0.f, 0.f, ColorDelta);
		static Color ClearColor(0.f, 0.f, 0.f, 1.f);

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
			AnimateClearColor(i_FrameTime);
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
	}
}