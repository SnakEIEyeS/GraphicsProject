#include "GameObject.h"

#include "../StaticMesh/StaticMesh.h"

Engine::Entity::GameObject::GameObject() : m_pStaticMesh(nullptr)
{
}

Engine::Entity::GameObject::~GameObject()
{
}

Engine::Entity::StaticMesh* Engine::Entity::GameObject::GetStaticMesh()
{
	return m_pStaticMesh;
}

void Engine::Entity::GameObject::SetStaticMesh(StaticMesh* i_pStaticMesh)
{
	m_pStaticMesh = i_pStaticMesh;
}


