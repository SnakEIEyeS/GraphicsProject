#pragma once

//class cyMatrix4f;

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
		//cyMatrix4f* m_pCameraMatrix;
		float m_YawAmount;
		float m_PitchAmount;
		float m_TranslateAmount;

	};
}
