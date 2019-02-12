#pragma once

#include <glad/glad.h>
#include <GL/glfw3.h>
//struct GLFWwindow;

#include "../cyCodeBase/cyGL.h"
#include "../Math/Vector3.h"

namespace cy
{
	class TriMesh;
}

namespace Engine
{
	namespace Lighting
	{
		class PointLight;
	}

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

		static Engine::Lighting::PointLight* RenderPointLight = nullptr;
		Engine::Lighting::PointLight& GetRenderPointLight();

		static const float AmbientConstant = 0.1f;
		static const float SpecularAlpha = 5.f;

		static const char* MaterialAmbientMapFile;
		static const char* MaterialDiffuseMapFile;
		static const char* MaterialSpecularMapFile;
		static float MaterialSpecularExponent;
		void SetMaterialDetails(cy::TriMesh* i_pTriMeshObj, int i_MaterialIndex);
	}
}