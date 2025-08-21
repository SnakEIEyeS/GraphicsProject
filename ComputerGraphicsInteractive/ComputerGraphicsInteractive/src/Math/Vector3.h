#pragma once

namespace Engine
{
	namespace Math
	{
		struct Vector3
		{
			float x, y, z;

			Vector3() : x(0.f), y(0.f), z(0.f)
			{}

			Vector3(float i_X, float i_Y, float i_Z) : x(i_X), y(i_Y), z(i_Z)
			{}
		};
	}
}
