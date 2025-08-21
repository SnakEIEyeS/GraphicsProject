#pragma once

namespace Engine
{
	namespace Math
	{
		const float PIFloat = 3.1415927f;

		bool AreAboutEqual(const float & i_lhs, const float & i_rhs, const float & epsilon = 0.000001f);
		float DegreesToRadians(const float & i_AngleInDegrees);
		float RadiansToDegrees(const float & i_AngleInRadians);
	}
}
