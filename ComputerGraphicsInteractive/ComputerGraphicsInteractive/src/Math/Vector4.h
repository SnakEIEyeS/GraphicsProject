#pragma once

namespace Engine
{
	namespace Math
	{
		struct Vector4
		{
			float x, y, z, w;

			Vector4() : x(0.f), y(0.f), z(0.f), w(0.f)
			{}

			Vector4(float i_X, float i_Y, float i_Z, float i_W) : x(i_X), y(i_Y), z(i_Z), w(i_W)
			{}
		};
	}
}