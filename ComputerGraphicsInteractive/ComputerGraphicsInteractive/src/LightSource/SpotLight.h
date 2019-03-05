#pragma once

namespace Engine
{
	namespace Entity
	{
		class GameObject;
	}

	namespace Lighting
	{
		class SpotLight
		{
		public:
			SpotLight();
			~SpotLight();

			SpotLight(Engine::Entity::GameObject* i_pGameObject);

			Engine::Entity::GameObject* GetGameObject() const;

		private:
			Engine::Entity::GameObject* m_pGameObject;
		};
	}
}
