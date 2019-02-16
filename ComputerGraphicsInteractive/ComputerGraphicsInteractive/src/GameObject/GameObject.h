#pragma once

namespace Engine
{
	namespace Entity
	{
		class StaticMesh;

		class GameObject
		{
		public:
			GameObject();
			~GameObject();

			StaticMesh* GetStaticMesh();
			void SetStaticMesh(StaticMesh* i_pStaticMesh);

		private:
			StaticMesh* m_pStaticMesh = nullptr;
		};
	}
}
