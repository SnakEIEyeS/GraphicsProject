#pragma once

#include <vector>

#include "../cyCodeBase/cyPoint.h"

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
			StaticMesh(GameObject* i_pGameObject, unsigned int i_VertexArrayID);

			const GameObject* GetGameObject() const;
			void SetGameObject(GameObject* i_pGameObject);

			unsigned int GetVertexArrayID();
			void SetVertexArrayID(unsigned int i_VertexArrayID);

			//TODO Think about what you actually need to store in StaticMesh
			std::vector<cy::Point3f>* m_pAVertexPositionsToDraw, m_pAVertexNormalsToDraw, m_pAVertexUVsToDraw;
			unsigned int m_AmbientTextureID, m_DiffuseTextureID, m_SpecularTextureID;

		private:
			GameObject* m_pGameObject;
			
			unsigned int m_VertexArrayID;
		};
	}
}
