#pragma once

#include <string>

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
			VertexTexture = 2
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

			//returns TextureID
			unsigned int CreateTexture2D(const char* i_TextureFileName, const unsigned int & i_TextureUnitIndex);
			//returns TextureID
			unsigned int CreateTextureCubeMap(std::string* i_ArrTextureFaceFileNames, const unsigned int & i_NumTextureFaces, const unsigned int & i_TextureUnitIndex);

		private:

		};

		ModelHandler* GetModelHandler();
	}
}
