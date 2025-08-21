#include "Camera.h"

#include "../cyCodeBase/cyMatrix.h"
#include "../GameObject/GameObject.h"

Engine::Entity::Camera::Camera() : m_pGameObject(nullptr)
{
}

Engine::Entity::Camera::~Camera()
{
	if (m_pGameObject)
	{
		delete m_pGameObject;
	}
}

Engine::Entity::Camera::Camera(GameObject * i_pGameObject) : m_pGameObject(i_pGameObject)
{
}

Engine::Entity::Camera::Camera(GameObject * i_pGameObject, float i_Fov, float i_ViewWidth, float i_ViewHeight, float i_ZNear, float i_ZFar)
	: m_pGameObject(i_pGameObject), m_Fov(i_Fov), m_ViewWidth(i_ViewWidth), m_ViewHeight(i_ViewHeight), m_ZNear(i_ZNear), m_ZFar(i_ZFar)
{
	SetPerspectiveProjection();
}

Engine::Entity::GameObject * Engine::Entity::Camera::GetGameObject() const
{
	return m_pGameObject;
}

void Engine::Entity::Camera::SetGameObject(GameObject * i_pGameObject)
{
	m_pGameObject = i_pGameObject;
}

cyMatrix4f Engine::Entity::Camera::GetPerspectiveProjection() const
{
	return m_PerspectiveProjectionMatrix;
}

void Engine::Entity::Camera::SetPerspectiveProjection()
{
	m_PerspectiveProjectionMatrix.SetPerspective(m_Fov, m_ViewWidth / m_ViewHeight, m_ZNear, m_ZFar);
}

void Engine::Entity::Camera::SetPerspectiveProjection(cyMatrix4f i_PerspectiveProjectionMatrix)
{
	m_PerspectiveProjectionMatrix = i_PerspectiveProjectionMatrix;
}

void Engine::Entity::Camera::SetPerspectiveProjection(float i_Fov, float i_Aspect, float i_ZNear, float i_ZFar)
{
	m_PerspectiveProjectionMatrix.SetPerspective(i_Fov, i_Aspect, i_ZNear, i_ZFar);
}

void Engine::Entity::Camera::SetPerspectiveProjection(float i_Fov, float i_ViewWidth, float i_ViewHeight, float i_ZNear, float i_ZFar)
{
	m_PerspectiveProjectionMatrix.SetPerspective(i_Fov, i_ViewWidth / i_ViewHeight, i_ZNear, i_ZFar);
}
