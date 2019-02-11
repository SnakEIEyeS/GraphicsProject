#include "PointLight.h"

#include "../cyCodeBase/cyPoint.h"

Engine::Lighting::PointLight::PointLight() : m_Position(cyPoint4f(0.f, 0.f, 0.f, 1.f))
{
}

Engine::Lighting::PointLight::~PointLight()
{
}

Engine::Lighting::PointLight::PointLight(cyPoint4f i_Position) : m_Position(i_Position)
{
}

cyPoint4f const Engine::Lighting::PointLight::GetPosition() const
{
	return m_Position;
}

void Engine::Lighting::PointLight::SetPosition(cyPoint4f i_Position)
{
	m_Position = i_Position;
}
