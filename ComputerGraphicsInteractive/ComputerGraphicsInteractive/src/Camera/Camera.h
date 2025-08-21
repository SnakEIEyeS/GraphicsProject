#pragma once

#include "../cyCodeBase/cyMatrix.h"

namespace Engine
{
	namespace Entity
	{
		class GameObject;

		class Camera
		{
		public:
			Camera();
			~Camera();

			Camera(GameObject* i_pGameObject);
			Camera(GameObject* i_pGameObject, float i_Fov, float i_ViewWidth, float i_ViewHeight, float i_ZNear, float i_ZFar);

			GameObject* GetGameObject() const;
			void SetGameObject(GameObject* i_pGameObject);

			cyMatrix4f GetPerspectiveProjection() const;
			void SetPerspectiveProjection();
			void SetPerspectiveProjection(cyMatrix4f i_PerspectiveProjectionMatrix);
			void SetPerspectiveProjection(float i_Fov, float i_Aspect, float i_ZNear, float i_ZFar);
			void SetPerspectiveProjection(float i_Fov, float i_ViewWidth, float i_ViewHeight, float i_ZNear, float i_ZFar);


			float m_Fov, m_ViewWidth, m_ViewHeight, m_ZNear, m_ZFar;

		private:
			cyMatrix4f m_PerspectiveProjectionMatrix;
			GameObject* m_pGameObject;
			
		};
	}
}

/*
namespace Camera
{
	class GraphicsCamera
	{
	public:
		GraphicsCamera();
		~GraphicsCamera();

		void YawCamera(bool i_PositiveRotation, float i_dt);
		void PitchCamera(bool i_PositiveRotation, float i_dt);
		void TranslateCamera(bool i_LeftTranslate, float i_dt);
		

	private:
		cyMatrix4f* m_pCameraMatrix;
		float m_YawAmount;
		float m_PitchAmount;
		float m_TranslateAmount;

	};
}
*/
