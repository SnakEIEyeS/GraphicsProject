#pragma once

#include <string>
#include <vector>

#include "../cyCodeBase/cyPoint.h"

namespace cy
{
	class TriMesh;
}


namespace Engine
{
	namespace ModelHandling
	{
		enum VertexInfo
		{
			VertexPosition = 0,
			VertexNormal = 1,
			VertexTexture = 2,
			VertexTangent = 3,
			VertexBitangent = 4
		};

		class ModelHandler
		{
		public:
			//returns VertexPositions Buffer ID
			unsigned int AddVertexPositions(cy::TriMesh* i_TriMeshObj, unsigned int i_VertexArrayID);
			
			//returns VertexNormals Buffer ID
			unsigned int AddVertexNormals(cy::TriMesh* i_TriMeshObj, unsigned int i_VertexArrayID);
			
			//returns VertexTextureCoordinates Buffer ID
			unsigned int AddVertexTextureCoordinates(cy::TriMesh* i_TriMeshObj, unsigned int i_VertexArrayID);
			
			//sets VertexTangents Buffer ID & VertexBitangents Buffer ID
			void CalculatePlaneTriTangentsBitangents(std::vector<cyPoint3f> i_PlaneVertexPositions, std::vector<cyPoint3f> i_PlaneUVs, unsigned int i_VertexArrayID, 
				unsigned int& o_VertexTangentArrayID, unsigned int& o_VertexBitangentArrayID);
			void CalculatePlaneQuadTangentsBitangents(std::vector<cyPoint3f> i_PlaneVertexPositions, std::vector<cyPoint3f> i_PlaneUVs, unsigned int i_VertexArrayID,
				unsigned int& o_VertexTangentArrayID, unsigned int& o_VertexBitangentArrayID);

			void CalculateTangentBitangent(cyPoint3f i_Edge1, cyPoint3f i_Edge2, cyPoint3f i_DeltaUV1, cyPoint3f i_DeltaUV2, cyPoint3f & o_Tangent, cyPoint3f & o_Bitangent);

			//returns TextureID
			unsigned int CreateTexture2D(const char* i_TextureFileName, const unsigned int & i_TextureUnitIndex);
			//returns TextureID
			unsigned int CreateTextureCubeMap(std::string i_ArrTextureFaceFileNames[], const unsigned int & i_NumTextureFaces, const unsigned int & i_TextureUnitIndex);
			unsigned int CreateTextureCubeMap(std::vector<std::string> i_ArrTextureFaceFileNames, const unsigned int & i_NumTextureFaces, const unsigned int & i_TextureUnitIndex);

			static const unsigned int NumPatchVertices = 4;

		private:

		};

		ModelHandler* GetModelHandler();
	}
}
