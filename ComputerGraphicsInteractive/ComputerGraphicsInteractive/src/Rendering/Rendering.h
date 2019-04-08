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
		class SpotLight;
	}

	namespace Rendering
	{
		static cyGLSLProgram* cyGraphicsProgram = nullptr;

		static const char* SceneVertexShaderFile = "res/EnvironmentMappingVS.shader";
		static const char* SceneFragmentShaderFile = "res/EnvironmentMappingFS.shader";

		static const char* RenderTextureVertexShaderFile = "res/RenderToTextureVS.shader";
		static const char* RenderTextureFragmentShaderFile = "res/RenderToTextureFS.shader";

		static const char* CubeMapTextureVertexShaderFile = "res/CubeMapVS.shader";
		static const char* CubeMapTextureFragmentShaderFile = "res/CubeMapFS.shader";

		static const char* QuadTessControlShaderFile = "res/QuadTCS.shader";
		static const char* QuadTessEvalShaderFile = "res/QuadTES.shader";

		static const char* DebugDrawVertexShaderFile = "res/DebugDrawVS.shader";
		static const char* DebugDrawGeometryShaderFile = "res/DebugDrawGS.shader"; 
		static const char* DebugDrawFragmentShaderFile = "res/DebugDrawFS.shader";
		
		
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
		//cyGLSLProgram* BuildProgram(const char* i_VertexShaderFile, const char* i_FragmentShaderFile);
		cyGLSLProgram* BuildProgram(const char* i_VertexShaderFile, const char* i_FragmentShaderFile, 
			const char* i_GeometryShaderFile = (const char*)nullptr, const char* i_TessControlShaderFile = (const char*)nullptr, const char* i_TessEvaluationShaderFile = (const char*)nullptr);
		bool BuildAndUseProgram();
		bool BuildAndUseProgram(const char* i_VertexShaderFile, const char* i_FragmentShaderFile);

		//TODO make PointLight have a GameObject
		static Engine::Lighting::PointLight* RenderPointLight = nullptr;
		Engine::Lighting::PointLight& GetRenderPointLight();

		static Engine::Lighting::SpotLight* RenderSpotLight = nullptr;
		Engine::Lighting::SpotLight& GetRenderSpotLight();

		static const float AmbientConstant = 0.1f;
		static const float SpecularAlpha = 50.f;

		static const char* MaterialAmbientMapFile;
		static const char* MaterialDiffuseMapFile;
		static const char* MaterialSpecularMapFile;
		static float MaterialSpecularExponent;
		void SetMaterialDetails(cy::TriMesh* i_pTriMeshObj, int i_MaterialIndex);

		void DecodeTexturePNG(std::string i_TextureFilename, std::vector<unsigned char> & o_ImageData, unsigned int & o_ImageWidth, unsigned int & o_ImageHeight);

		float GetMaxAnisotropicLevel();
	}
}