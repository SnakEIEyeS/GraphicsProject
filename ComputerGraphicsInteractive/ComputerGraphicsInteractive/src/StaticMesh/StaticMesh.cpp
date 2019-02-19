#include "StaticMesh.h"

#include "../GameObject/GameObject.h"

Engine::Entity::StaticMesh::StaticMesh() : m_pGameObject(nullptr)
{
}

Engine::Entity::StaticMesh::~StaticMesh()
{
	if (m_pGameObject)
	{
		delete m_pGameObject;
	}
}

Engine::Entity::StaticMesh::StaticMesh(GameObject* i_pGameObject) : m_pGameObject(i_pGameObject)
{
}

const Engine::Entity::GameObject * Engine::Entity::StaticMesh::GetGameObject() const
{
	return m_pGameObject;
}

void Engine::Entity::StaticMesh::SetGameObject(GameObject * i_pGameObject)
{
	m_pGameObject = i_pGameObject;
}

unsigned int Engine::Entity::StaticMesh::GetVertexArrayID()
{
	return m_VertexArrayID;
}

void Engine::Entity::StaticMesh::SetVertexArrayID(unsigned int i_VertexArrayID)
{
	m_VertexArrayID = i_VertexArrayID;
}

unsigned int Engine::Entity::StaticMesh::GetTextureID()
{
	return m_TextureID;
}

void Engine::Entity::StaticMesh::SetTextureID(unsigned int i_TextureID)
{
	m_TextureID = i_TextureID;
}
