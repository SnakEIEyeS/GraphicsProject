#include "MathUtility.h"

#include <math.h>

namespace Engine
{
	namespace Math
	{
		bool Math::AreAboutEqual(const float & i_lhs, const float & i_rhs, const float & epsilon)
		{
			if (fabs(i_lhs - i_rhs) > epsilon)
			{
				return false;
			}
			return true;
		}
		float DegreesToRadians(const float & i_AngleInDegrees)
		{
			return i_AngleInDegrees * PIFloat/180.f;
		}
		float RadiansToDegrees(const float & i_AngleInRadians)
		{
			return i_AngleInRadians * 180.f/PIFloat;
		}
	}
}
