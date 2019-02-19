#pragma once

struct GLFWwindow;

namespace Engine
{
	namespace Entity
	{
		class GameObject;
	}

	namespace Input
	{
		struct MousePosition
		{
			double x, y;

			MousePosition() {}
			~MousePosition() {}
			MousePosition(double i_x, double i_y) : x(i_x), y(i_y)
			{}
		};

		
		static const float MaxRotateAmount = 90.f;
		static const float MaxTranslateAmount = 40.f;
		static bool LeftMouseButtonHeldDown = false;
		static bool RightMouseButtonHeldDown = false;

		bool Startup();
		bool Shutdown();

		void Update(GLFWwindow* window, float i_FrameTime);
		void ProcessInput(GLFWwindow * window, float i_FrameTime);
		void CheckMouseState(GLFWwindow * window);
		void RotateBoundObject(GLFWwindow * window, float i_FrameTime);
		void TranslateBoundObject(GLFWwindow * window, float i_FrameTime);
		MousePosition GetMouseDispaclementInFrame(GLFWwindow * window);

		void SetCameraGameObject(Engine::Entity::GameObject* i_pGameObject);
		void SetCtrlBoundGameObject(Engine::Entity::GameObject* i_pGameObject);
		void SetAltBoundGameObject(Engine::Entity::GameObject* i_pGameObject);
	} //namespace Input
} //namespace Engine
