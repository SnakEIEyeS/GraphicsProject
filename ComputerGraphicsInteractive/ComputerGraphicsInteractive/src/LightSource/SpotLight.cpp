#include "SpotLight.h"

#include "../GameObject/GameObject.h"

Engine::Lighting::SpotLight::SpotLight() : m_pGameObject(nullptr)
{
}

Engine::Lighting::SpotLight::~SpotLight()
{
}

Engine::Lighting::SpotLight::SpotLight(Engine::Entity::GameObject * i_pGameObject) : m_pGameObject(i_pGameObject)
{
}

Engine::Entity::GameObject * Engine::Lighting::SpotLight::GetGameObject() const
{
	return m_pGameObject;
}
