#include "ModelHandler.h"

#include <assert.h>

#include <glad/glad.h>
#include <GL/glfw3.h>

#include "../cyCodeBase/cyTriMesh.h"
#include "../Rendering/Rendering.h"

namespace Engine
{
	namespace ModelHandling
	{
		static ModelHandler* ModelHandlerRef = nullptr;

		unsigned int ModelHandler::AddVertexPositions(cy::TriMesh * i_TriMeshObj, unsigned int i_VertexArrayID)
		{
			glBindVertexArray(i_VertexArrayID);

			//Vertex Positions
			unsigned int vertexPosBufferID;
			glGenBuffers(1, &vertexPosBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, vertexPosBufferID);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(TriMeshObj->V(0)) * TriMeshObj->NV(), const_cast<void*>(reinterpret_cast<void*>(&TriMeshObj->V(0))), GL_STATIC_DRAW);

			assert(sizeof(i_TriMeshObj->F(0).v) / sizeof(i_TriMeshObj->F(0).v[0]) == 3);
			const int TriVertsToDraw = i_TriMeshObj->NF() * sizeof(i_TriMeshObj->F(0).v) / sizeof(i_TriMeshObj->F(0).v[0]);
			
			std::vector<cyPoint3f> AVertexPositions;
			AVertexPositions.reserve(TriVertsToDraw);
			for (unsigned int i = 0; i < i_TriMeshObj->NF(); i++)
			{
				AVertexPositions.push_back(i_TriMeshObj->V(i_TriMeshObj->F(i).v[0]));
				AVertexPositions.push_back(i_TriMeshObj->V(i_TriMeshObj->F(i).v[1]));
				AVertexPositions.push_back(i_TriMeshObj->V(i_TriMeshObj->F(i).v[2]));
				//std::cout << i << " " << AVertexPositions[i].x << " " << AVertexPositions[i].y << " " << AVertexPositions[i].z << "\n";
			}
			glBufferData(GL_ARRAY_BUFFER, sizeof(i_TriMeshObj->V(0)) * TriVertsToDraw, const_cast<void*>(reinterpret_cast<void*>(&AVertexPositions[0])), GL_STATIC_DRAW);

			glEnableVertexAttribArray(VertexInfo::VertexPosition);
			glVertexAttribPointer(VertexInfo::VertexPosition, 3, GL_FLOAT, GL_FALSE, sizeof(i_TriMeshObj->V(0)), (const void*)0);

			return vertexPosBufferID;
		}

		unsigned int ModelHandler::AddVertexNormals(cy::TriMesh * i_TriMeshObj, unsigned int i_VertexArrayID)
		{
			glBindVertexArray(i_VertexArrayID);

			//Vertex Normals
			unsigned int vertexNormalBufferID;
			glGenBuffers(1, &vertexNormalBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, vertexNormalBufferID);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(TriMeshObj->VN(0))*TriMeshObj->NVN(), const_cast<void*>(reinterpret_cast<void*>(&TriMeshObj->VN(0))), GL_STATIC_DRAW);

			assert(sizeof(i_TriMeshObj->F(0).v) / sizeof(i_TriMeshObj->F(0).v[0]) == 3);
			const int TriVertsToDraw = i_TriMeshObj->NF() * sizeof(i_TriMeshObj->F(0).v) / sizeof(i_TriMeshObj->F(0).v[0]);

			std::vector<cyPoint3f> AVertexNormals;
			AVertexNormals.reserve(TriVertsToDraw);
			for (unsigned int i = 0; i < i_TriMeshObj->NF(); i++)
			{
				AVertexNormals.push_back(i_TriMeshObj->VN(i_TriMeshObj->F(i).v[0]));
				AVertexNormals.push_back(i_TriMeshObj->VN(i_TriMeshObj->F(i).v[1]));
				AVertexNormals.push_back(i_TriMeshObj->VN(i_TriMeshObj->F(i).v[2]));
			}
			glBufferData(GL_ARRAY_BUFFER, sizeof(i_TriMeshObj->VN(0)) * TriVertsToDraw, const_cast<void*>(reinterpret_cast<void*>(&AVertexNormals[0])), GL_STATIC_DRAW);

			glEnableVertexAttribArray(VertexInfo::VertexNormal);
			glVertexAttribPointer(VertexInfo::VertexNormal, sizeof(i_TriMeshObj->VN(0)) / sizeof(i_TriMeshObj->VN(0).x), GL_FLOAT, GL_FALSE, sizeof(i_TriMeshObj->VN(0)), (const void*)0);

			return vertexNormalBufferID;
		}

		unsigned int ModelHandler::AddVertexTextureCoordinates(cy::TriMesh * i_TriMeshObj, unsigned int i_VertexArrayID)
		{
			glBindVertexArray(i_VertexArrayID);

			//Vertex Texture Coordinates
			unsigned int vertexTextureBufferID;
			glGenBuffers(1, &vertexTextureBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, vertexTextureBufferID);

			assert(sizeof(i_TriMeshObj->F(0).v) / sizeof(i_TriMeshObj->F(0).v[0]) == 3);
			const int TriVertsToDraw = i_TriMeshObj->NF() * sizeof(i_TriMeshObj->F(0).v) / sizeof(i_TriMeshObj->F(0).v[0]);

			std::vector<cyPoint3f> AVertexTextures;
			AVertexTextures.reserve(TriVertsToDraw);
			const int NumVertexTextures = i_TriMeshObj->NVT();
			for (unsigned int i = 0; i < i_TriMeshObj->NF(); i++)
			{
				AVertexTextures.push_back(i_TriMeshObj->VT(i_TriMeshObj->FT(i).v[0]));
				AVertexTextures.push_back(i_TriMeshObj->VT(i_TriMeshObj->FT(i).v[1]));
				AVertexTextures.push_back(i_TriMeshObj->VT(i_TriMeshObj->FT(i).v[2]));
			}
			glBufferData(GL_ARRAY_BUFFER, sizeof(i_TriMeshObj->VT(0))*TriVertsToDraw, const_cast<void*>(reinterpret_cast<void*>(&AVertexTextures[0])), GL_STATIC_DRAW);

			glEnableVertexAttribArray(VertexInfo::VertexTexture);
			glVertexAttribPointer(VertexInfo::VertexTexture, sizeof(i_TriMeshObj->VT(0)) / sizeof(i_TriMeshObj->VT(0).x), GL_FLOAT, GL_FALSE, sizeof(i_TriMeshObj->VT(0)), (const void*)0);

			return vertexTextureBufferID;
		}

		unsigned int ModelHandler::CreateTexture2D(const char* i_TextureFileName, const unsigned int &  i_TextureUnitIndex)
		{
			std::vector<unsigned char> TextureImageData;
			unsigned int TextureWidth;
			unsigned int TextureHeight;
			Engine::Rendering::DecodeTexturePNG(i_TextureFileName, TextureImageData, TextureWidth, TextureHeight);

			glActiveTexture(GL_TEXTURE0 + i_TextureUnitIndex);
			GLuint err = glGetError();
			if (err != 0)
			{
				printf("Error code: %d\n", err);
			}
			unsigned int CreatedTextureID;
			glGenTextures(1, &CreatedTextureID);
			err = glGetError();
			if (err != 0)
			{
				printf("Error code: %d\n", err);
			}
			glBindTexture(GL_TEXTURE_2D, CreatedTextureID);
			err = glGetError();
			if (err != 0)
			{
				printf("Error code: %d\n", err);
			}
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextureWidth, TextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)&TextureImageData[0]);
			err = glGetError();
			if (err != 0)
			{
				printf("Error code: %d\n", err);
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			err = glGetError();
			if (err != 0)
			{
				printf("Error code: %d\n", err);
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			err = glGetError();
			if (err != 0)
			{
				printf("Error code: %d\n", err);
			}
			glGenerateMipmap(GL_TEXTURE_2D);
			err = glGetError();
			if (err != 0)
			{
				printf("Error code: %d\n", err);
			}
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, Engine::Rendering::GetMaxAnisotropicLevel());
			err = glGetError();
			if (err != 0)
			{
				printf("Error code: %d\n", err);
			}
			
			return CreatedTextureID;
		}

		unsigned int ModelHandler::CreateTextureCubeMap(std::string i_ArrTextureFaceFileNames[], const unsigned int & i_NumTextureFaces, const unsigned int & i_TextureUnitIndex)
		{
			glActiveTexture(GL_TEXTURE0 + i_TextureUnitIndex);
			unsigned int CreatedCubeMapTextureID;
			glGenTextures(1, &CreatedCubeMapTextureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, CreatedCubeMapTextureID);

			std::vector<unsigned char> CubeMapTextureFaceData[6];
			unsigned int CubeMapFaceWidth;
			unsigned int CubeMapFaceHeight;

			for (unsigned int i = 0; i < i_NumTextureFaces; i++)
			{
				Engine::Rendering::DecodeTexturePNG(i_ArrTextureFaceFileNames[i], CubeMapTextureFaceData[i], CubeMapFaceWidth, CubeMapFaceHeight);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, CubeMapFaceWidth, CubeMapFaceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)&CubeMapTextureFaceData[i][0]);
				//std::cout << CubeMapFaceWidth << " " << CubeMapFaceHeight << "\n";
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY, Engine::Rendering::GetMaxAnisotropicLevel());

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			return CreatedCubeMapTextureID;
		}

		unsigned int ModelHandler::CreateTextureCubeMap(std::vector<std::string> i_ArrTextureFaceFileNames, const unsigned int & i_NumTextureFaces, const unsigned int & i_TextureUnitIndex)
		{
			glActiveTexture(GL_TEXTURE0 + i_TextureUnitIndex);
			unsigned int CreatedCubeMapTextureID;
			glGenTextures(1, &CreatedCubeMapTextureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, CreatedCubeMapTextureID);

			std::vector<unsigned char> CubeMapTextureFaceData[6];
			unsigned int CubeMapFaceWidth;
			unsigned int CubeMapFaceHeight;

			for (unsigned int i = 0; i < i_NumTextureFaces; i++)
			{
				Engine::Rendering::DecodeTexturePNG(i_ArrTextureFaceFileNames[i], CubeMapTextureFaceData[i], CubeMapFaceWidth, CubeMapFaceHeight);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, CubeMapFaceWidth, CubeMapFaceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)&CubeMapTextureFaceData[i][0]);
				//std::cout << CubeMapFaceWidth << " " << CubeMapFaceHeight << "\n";
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY, Engine::Rendering::GetMaxAnisotropicLevel());

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			return CreatedCubeMapTextureID;
		}


		ModelHandler* GetModelHandler()
		{
			if (!ModelHandlerRef)
			{
				ModelHandlerRef = new ModelHandler();
			}

			return ModelHandlerRef;
		}
	}
}
