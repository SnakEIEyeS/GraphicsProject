#include "Rendering.h"

#include <assert.h>
#include <iostream>

#include <glad/glad.h>
#include <GL/glfw3.h>

#include "../cyCodeBase/cyGL.h"
#include "../cyCodeBase/cyPoint.h"
#include "../cyCodeBase/cyTriMesh.h"
#include "../FileHandling/lodepng.h"
#include "../Math/MathUtility.h"
#include "../LightSource/PointLight.h"
#include "Renderer.h"

namespace Engine
{
	namespace Rendering
	{
		

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
				//std::cout << "Recompiling shaders...\n";
				//Engine::Rendering::BuildAndUseProgram();
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

		cyGLSLProgram * BuildProgram(const char * i_VertexShaderFile, const char * i_FragmentShaderFile)
		{
			cyGLSLProgram* ProgramToReturn = new cyGLSLProgram();
			ProgramToReturn->BuildFiles(i_VertexShaderFile, i_FragmentShaderFile, nullptr, nullptr, nullptr);
			return ProgramToReturn;
		}

		bool BuildAndUseProgram()
		{
			bool built = GetGLProgram()->BuildFiles(SceneVertexShaderFile, SceneFragmentShaderFile, nullptr, nullptr, nullptr);
			assert(built);
			GetGLProgram()->Bind();
			return true;
		}

		bool BuildAndUseProgram(const char * i_VertexShaderFile, const char * i_FragmentShaderFile)
		{
			bool built = GetGLProgram()->BuildFiles(i_VertexShaderFile, i_VertexShaderFile, nullptr, nullptr, nullptr);
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

		void SetMaterialDetails(cy::TriMesh * i_pTriMeshObj, int i_MaterialIndex)
		{
			Engine::Rendering::MaterialAmbientMapFile = i_pTriMeshObj->M(i_MaterialIndex).map_Ka;
			Engine::Rendering::MaterialDiffuseMapFile = i_pTriMeshObj->M(i_MaterialIndex).map_Kd;
			Engine::Rendering::MaterialSpecularMapFile = i_pTriMeshObj->M(i_MaterialIndex).map_Ks;
			Engine::Rendering::MaterialSpecularExponent = i_pTriMeshObj->M(i_MaterialIndex).Ns;
		}

		void DecodeTexturePNG(std::string i_TextureFilename, std::vector<unsigned char> & o_ImageData, unsigned int & o_ImageWidth, unsigned int & o_ImageHeight)
		{
			//decode
			unsigned int error = lodepng::decode(o_ImageData, o_ImageWidth, o_ImageHeight, i_TextureFilename);

			//if there's an error, display it
			if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

			//the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
		}

		float GetMaxAnisotropicLevel()
		{
			float MaxAnisotropicLevel;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &MaxAnisotropicLevel);
			return MaxAnisotropicLevel;
		}

	}
}



/*
References:
https://raw.githubusercontent.com/lvandeve/lodepng/master/examples/example_decode.cpp for DecodeTexturePNG()
*/