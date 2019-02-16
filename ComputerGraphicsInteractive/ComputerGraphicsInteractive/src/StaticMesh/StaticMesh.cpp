#include "StaticMesh.h"

Engine::Entity::StaticMesh::StaticMesh()
{
}

Engine::Entity::StaticMesh::~StaticMesh()
{
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
