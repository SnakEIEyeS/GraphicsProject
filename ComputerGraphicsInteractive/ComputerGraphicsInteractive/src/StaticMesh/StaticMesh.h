#pragma once

namespace Engine
{
	namespace Entity
	{
		class StaticMesh
		{
		public:
			StaticMesh();
			~StaticMesh();

			unsigned int GetVertexArrayID();
			void SetVertexArrayID(unsigned int i_VertexArrayID);

			unsigned int GetTextureID();
			void SetTextureID(unsigned int i_TextureID);

		private:
			unsigned int m_VertexArrayID;
			unsigned int m_TextureID;
		};
	}
}
