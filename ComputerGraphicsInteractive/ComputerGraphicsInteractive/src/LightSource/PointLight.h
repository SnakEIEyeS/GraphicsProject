#pragma once

#include "../cyCodeBase/cyPoint.h"

namespace Engine
{
	namespace Lighting
	{
		class PointLight
		{
		public:
			PointLight();
			~PointLight();

			PointLight(cyPoint4f i_Position);

			cyPoint4f const GetPosition() const;
			void SetPosition(cyPoint4f i_Position);

		private:
			cyPoint4f m_Position;
		};
	}
}
