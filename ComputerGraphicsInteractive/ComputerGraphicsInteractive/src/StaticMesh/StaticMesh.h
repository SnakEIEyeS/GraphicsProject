#pragma once

namespace Engine
{
	namespace Entity
	{
		class GameObject;

		class StaticMesh
		{
		public:
			StaticMesh();
			~StaticMesh();

			StaticMesh(GameObject* i_pGameObject);

			const GameObject* GetGameObject() const;
			void SetGameObject(GameObject* i_pGameObject);

			unsigned int GetVertexArrayID();
			void SetVertexArrayID(unsigned int i_VertexArrayID);

			unsigned int GetTextureID();
			void SetTextureID(unsigned int i_TextureID);

		private:
			GameObject* m_pGameObject;
			unsigned int m_VertexArrayID;
			unsigned int m_TextureID;	//TODO split into 3 texture types
		};
	}
}
