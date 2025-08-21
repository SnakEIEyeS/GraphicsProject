#pragma once

#include "../cyCodeBase/cyPoint.h"
#include "../cyCodeBase/cyMatrix.h"

namespace Engine
{
	namespace Entity
	{
		class GameObject
		{
		public:
			GameObject();
			~GameObject();

			GameObject(cyPoint3f i_Position);
			GameObject(cyPoint3f i_Position, cyPoint3f i_Rotation);

			const cyPoint3f GetPosition() const;
			void SetPosition(cyPoint3f i_Position);

			const cyPoint3f GetRotation() const;
			void SetRotation(cyPoint3f i_Rotation);

			cyMatrix4f GetTransform() const;

		private:
			cyPoint3f m_Position;
			cyPoint3f m_Rotation;
		};
	}
}
