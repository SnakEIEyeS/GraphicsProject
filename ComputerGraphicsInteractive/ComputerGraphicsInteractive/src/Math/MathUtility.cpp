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
	}
}
