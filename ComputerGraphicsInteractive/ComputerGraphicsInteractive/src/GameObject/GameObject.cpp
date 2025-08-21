#include "GameObject.h"

#include "../cyCodeBase/cyMatrix.h"
#include "../cyCodeBase/cyPoint.h"
#include "../Math/MathUtility.h"

Engine::Entity::GameObject::GameObject()
{
}

Engine::Entity::GameObject::~GameObject()
{
}

Engine::Entity::GameObject::GameObject(cyPoint3f i_Position) : m_Position(i_Position), m_Rotation(cyPoint3f(0.f, 0.f, 0.f))
{
}

Engine::Entity::GameObject::GameObject(cyPoint3f i_Position, cyPoint3f i_Rotation) : m_Position(i_Position), m_Rotation(i_Rotation)
{
}

const cyPoint3f Engine::Entity::GameObject::GetPosition() const
{
	return m_Position;
}

void Engine::Entity::GameObject::SetPosition(cyPoint3f i_Position)
{
	m_Position = i_Position;
}

const cyPoint3f Engine::Entity::GameObject::GetRotation() const
{
	return m_Rotation;
}

void Engine::Entity::GameObject::SetRotation(cyPoint3f i_Rotation)
{
	m_Rotation = i_Rotation;
}

cyMatrix4f Engine::Entity::GameObject::GetTransform() const
{
	cyMatrix4f TransformMatrixToReturn, YawMatrix, PitchMatrix, RollMatrix;
	YawMatrix.SetRotationY(Engine::Math::DegreesToRadians(m_Rotation.y));
	PitchMatrix.SetRotationX(Engine::Math::DegreesToRadians(m_Rotation.x));
	RollMatrix.SetRotationZ(Engine::Math::DegreesToRadians(m_Rotation.z));

	TransformMatrixToReturn = RollMatrix * PitchMatrix * YawMatrix;
	TransformMatrixToReturn.AddTrans(m_Position);

	return TransformMatrixToReturn;
}

