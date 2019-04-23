#include "ModelHandler.h"

#include <assert.h>

#include <glad/glad.h>
#include <GL/glfw3.h>

#include "../cyCodeBase/cyPoint.h"
#include "../cyCodeBase/cyTriMesh.h"
#include "../Rendering/Rendering.h"
#include "../StaticMesh/StaticMesh.h"


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

		void ModelHandler::CalculatePlaneTriTangentsBitangents(std::vector<cyPoint3f> i_PlaneVertexPositions, std::vector<cyPoint3f> i_PlaneUVs, unsigned int i_VertexArrayID, 
			unsigned int & o_VertexTangentArrayID, unsigned int & o_VertexBitangentArrayID)
		{

			const unsigned int NumPlaneVertices = 6;

			assert(i_PlaneVertexPositions.size() == NumPlaneVertices);
			assert(i_PlaneUVs.size() == NumPlaneVertices);

			std::vector<cyPoint3f> APlaneTangents;
			std::vector<cyPoint3f> APlaneBitangents;

			unsigned int TriFirstVertex = 0;

			for (int i = 0; i < 2; i++)
			{
				cyPoint3f Edge1 = i_PlaneVertexPositions[TriFirstVertex + 1] - i_PlaneVertexPositions[TriFirstVertex];
				cyPoint3f Edge2 = i_PlaneVertexPositions[TriFirstVertex + 2] - i_PlaneVertexPositions[TriFirstVertex];

				cyPoint3f DeltaUV1 = i_PlaneUVs[TriFirstVertex + 1] - i_PlaneUVs[TriFirstVertex];
				cyPoint3f DeltaUV2 = i_PlaneUVs[TriFirstVertex + 2] - i_PlaneUVs[TriFirstVertex];

				cyPoint3f Tangent;
				cyPoint3f Bitangent;
				
				/*float f = 1.f / (DeltaUV1.x * DeltaUV2.y - DeltaUV2.x * DeltaUV1.y);
				
				Tangent.x = f * (DeltaUV2.y * Edge1.x - DeltaUV1.y * Edge2.x);
				Tangent.y = f * (DeltaUV2.y * Edge1.y - DeltaUV1.y * Edge2.y);
				Tangent.z = f * (DeltaUV2.y * Edge1.z - DeltaUV1.y * Edge2.z);
				Tangent.Normalize();

				
				Bitangent.x = f * (-DeltaUV2.x * Edge1.x + DeltaUV1.x * Edge2.x);
				Bitangent.y = f * (-DeltaUV2.x * Edge1.y + DeltaUV1.x * Edge2.y);
				Bitangent.z = f * (-DeltaUV2.x * Edge1.z + DeltaUV1.x * Edge2.z);
				Bitangent.Normalize();*/

				CalculateTangentBitangent(Edge1, Edge2, DeltaUV1, DeltaUV2, Tangent, Bitangent);

				APlaneTangents.push_back(Tangent);
				APlaneTangents.push_back(Tangent);
				APlaneTangents.push_back(Tangent);

				APlaneBitangents.push_back(Bitangent);
				APlaneBitangents.push_back(Bitangent);
				APlaneBitangents.push_back(Bitangent);

				TriFirstVertex += 3;
			}

			assert(APlaneTangents.size() == NumPlaneVertices);
			assert(APlaneBitangents.size() == NumPlaneVertices);

			glBindVertexArray(i_VertexArrayID);

			//Add Vertex Tangent Coordinates through OpenGL
			unsigned int vertexTangentBufferID;
			glGenBuffers(1, &vertexTangentBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, vertexTangentBufferID);

			glBufferData(GL_ARRAY_BUFFER, sizeof(APlaneTangents[0]) * NumPlaneVertices, const_cast<void*>(reinterpret_cast<void*>(&APlaneTangents[0])), GL_STATIC_DRAW);

			glEnableVertexAttribArray(VertexInfo::VertexTangent);
			glVertexAttribPointer(VertexInfo::VertexTangent, sizeof(APlaneTangents[0]) / sizeof(APlaneTangents[0].x), GL_FLOAT, GL_FALSE, sizeof(APlaneTangents[0]), (const void*)0);

			o_VertexTangentArrayID = vertexTangentBufferID;


			glBindVertexArray(i_VertexArrayID);

			//Add Vertex Bitangent Coordinates through OpenGL
			unsigned int vertexBitangentBufferID;
			glGenBuffers(1, &vertexBitangentBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBitangentBufferID);

			glBufferData(GL_ARRAY_BUFFER, sizeof(APlaneBitangents[0]) * NumPlaneVertices, const_cast<void*>(reinterpret_cast<void*>(&APlaneBitangents[0])), GL_STATIC_DRAW);

			glEnableVertexAttribArray(VertexInfo::VertexBitangent);
			glVertexAttribPointer(VertexInfo::VertexBitangent, sizeof(APlaneBitangents[0]) / sizeof(APlaneBitangents[0].x), GL_FLOAT, GL_FALSE, sizeof(APlaneBitangents[0]), (const void*)0);

			o_VertexBitangentArrayID = vertexBitangentBufferID;
			
		}

		void ModelHandler::CalculatePlaneQuadTangentsBitangents(std::vector<cyPoint3f> i_PlaneVertexPositions, std::vector<cyPoint3f> i_PlaneUVs, unsigned int i_VertexArrayID, unsigned int & o_VertexTangentArrayID, unsigned int & o_VertexBitangentArrayID)
		{
			const unsigned int NumPlaneVertices = 4;

			assert(i_PlaneVertexPositions.size() == NumPlaneVertices);
			assert(i_PlaneUVs.size() == NumPlaneVertices);

			std::vector<cyPoint3f> APlaneTangents;
			std::vector<cyPoint3f> APlaneBitangents;

			//Calculate for first 'Triangle' in Quad
			cyPoint3f Edge1 = i_PlaneVertexPositions[1] - i_PlaneVertexPositions[0];
			cyPoint3f Edge2 = i_PlaneVertexPositions[2] - i_PlaneVertexPositions[0];

			cyPoint3f DeltaUV1 = i_PlaneUVs[1] - i_PlaneUVs[0];
			cyPoint3f DeltaUV2 = i_PlaneUVs[2] - i_PlaneUVs[0];

			cyPoint3f Tangent;
			cyPoint3f Bitangent;
			CalculateTangentBitangent(Edge1, Edge2, DeltaUV1, DeltaUV2, Tangent, Bitangent);

			APlaneTangents.push_back(Tangent);
			APlaneTangents.push_back(Tangent);
			APlaneTangents.push_back(Tangent);

			APlaneBitangents.push_back(Bitangent);
			APlaneBitangents.push_back(Bitangent);
			APlaneBitangents.push_back(Bitangent);

			//Calculate for second 'Triangle' in Quad
			Edge1 = i_PlaneVertexPositions[0] - i_PlaneVertexPositions[3];
			Edge2 = i_PlaneVertexPositions[2] - i_PlaneVertexPositions[3];

			DeltaUV1 = i_PlaneUVs[0] - i_PlaneUVs[3];
			DeltaUV2 = i_PlaneUVs[2] - i_PlaneUVs[3];

			CalculateTangentBitangent(Edge1, Edge2, DeltaUV1, DeltaUV2, Tangent, Bitangent);

			APlaneTangents.push_back(Tangent);
			APlaneBitangents.push_back(Bitangent);

			assert(APlaneTangents.size() == NumPlaneVertices);
			assert(APlaneBitangents.size() == NumPlaneVertices);

			glBindVertexArray(i_VertexArrayID);

			//Add Vertex Tangent Coordinates through OpenGL
			unsigned int vertexTangentBufferID;
			glGenBuffers(1, &vertexTangentBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, vertexTangentBufferID);

			glBufferData(GL_ARRAY_BUFFER, sizeof(APlaneTangents[0]) * NumPlaneVertices, const_cast<void*>(reinterpret_cast<void*>(&APlaneTangents[0])), GL_STATIC_DRAW);

			glEnableVertexAttribArray(VertexInfo::VertexTangent);
			glVertexAttribPointer(VertexInfo::VertexTangent, sizeof(APlaneTangents[0]) / sizeof(APlaneTangents[0].x), GL_FLOAT, GL_FALSE, sizeof(APlaneTangents[0]), (const void*)0);

			o_VertexTangentArrayID = vertexTangentBufferID;


			glBindVertexArray(i_VertexArrayID);

			//Add Vertex Bitangent Coordinates through OpenGL
			unsigned int vertexBitangentBufferID;
			glGenBuffers(1, &vertexBitangentBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBitangentBufferID);

			glBufferData(GL_ARRAY_BUFFER, sizeof(APlaneBitangents[0]) * NumPlaneVertices, const_cast<void*>(reinterpret_cast<void*>(&APlaneBitangents[0])), GL_STATIC_DRAW);

			glEnableVertexAttribArray(VertexInfo::VertexBitangent);
			glVertexAttribPointer(VertexInfo::VertexBitangent, sizeof(APlaneBitangents[0]) / sizeof(APlaneBitangents[0].x), GL_FLOAT, GL_FALSE, sizeof(APlaneBitangents[0]), (const void*)0);

			o_VertexBitangentArrayID = vertexBitangentBufferID;
		}

		void ModelHandler::CalculateTangentBitangent(cyPoint3f i_Edge1, cyPoint3f i_Edge2, cyPoint3f i_DeltaUV1, cyPoint3f i_DeltaUV2, cyPoint3f & o_Tangent, cyPoint3f & o_Bitangent)
		{
			float f = 1.f / (i_DeltaUV1.x * i_DeltaUV2.y - i_DeltaUV2.x * i_DeltaUV1.y);
			o_Tangent.x = f * (i_DeltaUV2.y * i_Edge1.x - i_DeltaUV1.y * i_Edge2.x);
			o_Tangent.y = f * (i_DeltaUV2.y * i_Edge1.y - i_DeltaUV1.y * i_Edge2.y);
			o_Tangent.z = f * (i_DeltaUV2.y * i_Edge1.z - i_DeltaUV1.y * i_Edge2.z);
			o_Tangent.Normalize();

			o_Bitangent.x = f * (-i_DeltaUV2.x * i_Edge1.x + i_DeltaUV1.x * i_Edge2.x);
			o_Bitangent.y = f * (-i_DeltaUV2.x * i_Edge1.y + i_DeltaUV1.x * i_Edge2.y);
			o_Bitangent.z = f * (-i_DeltaUV2.x * i_Edge1.z + i_DeltaUV1.x * i_Edge2.z);
			o_Bitangent.Normalize();
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

		void ModelHandler::CreateXYPlaneClipSpaceTris(Engine::Entity::StaticMesh* i_pPlaneStaticMesh, float i_LeftExtent, float i_RightExtent, float i_BottomExtent, float i_TopExtent)
		{
			//Plane VAO
			unsigned int PlaneVertexArrayID;
			glGenVertexArrays(1, &PlaneVertexArrayID);
			glBindVertexArray(PlaneVertexArrayID);
			i_pPlaneStaticMesh->SetVertexArrayID(PlaneVertexArrayID);

			//Plane VertexPositions
			std::vector<cyPoint3f> APlaneVertPos;
			APlaneVertPos.reserve(6);

			cyPoint3f PlaneTopLeft(i_LeftExtent, i_TopExtent, 0.f);
			cyPoint3f PlaneTopRight(i_RightExtent, i_TopExtent, 0.f);
			cyPoint3f PlaneBottomRight(i_RightExtent, i_BottomExtent, 0.f);
			cyPoint3f PlaneBottomLeft(i_LeftExtent, i_BottomExtent, 0.f);

			APlaneVertPos.push_back(PlaneTopLeft);
			APlaneVertPos.push_back(PlaneBottomLeft);
			APlaneVertPos.push_back(PlaneBottomRight);
			APlaneVertPos.push_back(PlaneBottomRight);
			APlaneVertPos.push_back(PlaneTopRight);
			APlaneVertPos.push_back(PlaneTopLeft);

			unsigned int planeVertexPosBufferID;
			glGenBuffers(1, &planeVertexPosBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, planeVertexPosBufferID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(APlaneVertPos[0]) * APlaneVertPos.size(), const_cast<void*>(reinterpret_cast<void*>(&APlaneVertPos[0])), GL_STATIC_DRAW);

			glEnableVertexAttribArray(Engine::ModelHandling::VertexInfo::VertexPosition);
			glVertexAttribPointer(Engine::ModelHandling::VertexInfo::VertexPosition, 3, GL_FLOAT, GL_FALSE, sizeof(APlaneVertPos[0]), (const void*)0);

			i_pPlaneStaticMesh->m_NumVertexPositions = APlaneVertPos.size();

			//Plane VertNormals
			std::vector<cyPoint3f> APlaneVertNormals;
			APlaneVertNormals.reserve(6);
			cyPoint3f UpVector(0.f, 0.f, 1.f);
			const unsigned int PlaneVertices = 6;
			for (int i = 0; i < PlaneVertices; i++)
			{
				APlaneVertNormals.push_back(UpVector);
			}

			unsigned int planeVertexNormalBufferID;
			glGenBuffers(1, &planeVertexNormalBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, planeVertexNormalBufferID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(APlaneVertNormals[0]) * APlaneVertNormals.size(), const_cast<void*>(reinterpret_cast<void*>(&APlaneVertNormals[0])), GL_STATIC_DRAW);
			glEnableVertexAttribArray(Engine::ModelHandling::VertexInfo::VertexNormal);
			glVertexAttribPointer(Engine::ModelHandling::VertexInfo::VertexNormal, sizeof(APlaneVertNormals[0]) / sizeof(APlaneVertNormals[0].x), GL_FLOAT, GL_FALSE, sizeof(APlaneVertNormals[0]), (const void*)0);

			//Plane TextureCoordinates
			std::vector<cyPoint3f> APlaneVertTextures;
			APlaneVertTextures.reserve(6);

			cyPoint3f PlaneUVTopLeft(0.f, 0.f, 0.f);
			cyPoint3f PlaneUVTopRight(1.f, 0.f, 0.f);
			cyPoint3f PlaneUVBottomRight(1.f, 1.f, 0.f);
			cyPoint3f PlaneUVBottomLeft(0.f, 1.f, 0.f);

			APlaneVertTextures.push_back(PlaneUVTopLeft);
			APlaneVertTextures.push_back(PlaneUVBottomLeft);
			APlaneVertTextures.push_back(PlaneUVBottomRight);
			APlaneVertTextures.push_back(PlaneUVBottomRight);
			APlaneVertTextures.push_back(PlaneUVTopRight);
			APlaneVertTextures.push_back(PlaneUVTopLeft);

			unsigned int planeVertexTextureBufferID;
			glGenBuffers(1, &planeVertexTextureBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, planeVertexTextureBufferID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(APlaneVertTextures[0])*APlaneVertTextures.size(), const_cast<void*>(reinterpret_cast<void*>(&APlaneVertTextures[0])), GL_STATIC_DRAW);
			glEnableVertexAttribArray(Engine::ModelHandling::VertexInfo::VertexTexture);
			glVertexAttribPointer(Engine::ModelHandling::VertexInfo::VertexTexture, sizeof(APlaneVertTextures[0]) / sizeof(APlaneVertTextures[0].x), GL_FLOAT, GL_FALSE, sizeof(APlaneVertTextures[0]), (const void*)0);

			unsigned int PlaneTangentsBufferID;
			unsigned int PlaneBitangentsBufferID;
			Engine::ModelHandling::GetModelHandler()->CalculatePlaneTriTangentsBitangents(APlaneVertPos, APlaneVertTextures, i_pPlaneStaticMesh->GetVertexArrayID(), PlaneTangentsBufferID, PlaneBitangentsBufferID);
			//Engine::ModelHandling::GetModelHandler()->CalculatePlaneQuadTangentsBitangents(APlaneVertPos, APlaneVertTextures, planeVertexArrayID, PlaneTangentsBufferID, PlaneBitangentsBufferID);

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



/*
*** References ***

https://learnopengl.com/Advanced-Lighting/Normal-Mapping for Tangent & Bitangent calculation

*/
