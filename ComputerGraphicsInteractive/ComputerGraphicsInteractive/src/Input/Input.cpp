#include "Input.h"

#include <assert.h>
#include <iostream>

#include <GL/glfw3.h>

#include "../cyCodeBase/cyPoint.h"
#include "../GameObject/GameObject.h"

namespace Engine
{
	namespace Input
	{
		MousePosition LastMousePosition;
		Engine::Entity::GameObject* CameraGameObject = nullptr;

		bool Startup()
		{
			std::cout << "Input Startup\n";
			return true;
		}

		bool Shutdown()
		{
			std::cout << "Input Shutdown\n";
			return true;
		}


		void Update(GLFWwindow * window, float i_FrameTime)
		{
			assert(window);
			ProcessInput(window, i_FrameTime);
		}

		void ProcessInput(GLFWwindow * window, float i_FrameTime)
		{
			assert(window);

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(window, true);
			}

			if (LeftMouseButtonHeldDown)
			{
				RotateBoundObject(window, i_FrameTime);
			}

			if (RightMouseButtonHeldDown)
			{
				TranslateBoundObject(window, i_FrameTime);
				
			}

			CheckMouseState(window);
		}

		void CheckMouseState(GLFWwindow * window)
		{
			assert(window);

			int LMBState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
			if (LMBState == GLFW_PRESS)
			{
				LeftMouseButtonHeldDown = true;
			}
			else if (LMBState == GLFW_RELEASE)
			{
				LeftMouseButtonHeldDown = false;
			}

			int RMBState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
			if (RMBState == GLFW_PRESS)
			{
				RightMouseButtonHeldDown = true;
			}
			else if (RMBState == GLFW_RELEASE)
			{
				RightMouseButtonHeldDown = false;
			}

			glfwGetCursorPos(window, &LastMousePosition.x, &LastMousePosition.y);
		}

		void RotateBoundObject(GLFWwindow * window, float i_FrameTime)
		{
			assert(window);
			MousePosition MouseDisplacementInFrame = GetMouseDispaclementInFrame(window);
			
			int WindowWidth, WindowHeight;
			glfwGetWindowSize(window, &WindowWidth, &WindowHeight);
			float YawAmount = (float)MouseDisplacementInFrame.x / WindowWidth * MaxRotateAmount * i_FrameTime;
			float PitchAmount = (float)MouseDisplacementInFrame.y / WindowHeight * MaxRotateAmount * i_FrameTime;

			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
			{
				//Rotate Ctrl bound object
			}
			else if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
			{
				//Rotate Alt bound object
			}
			else
			{
				//Rotate Camera
				if (CameraGameObject)
				{
					CameraGameObject->SetRotation(CameraGameObject->GetRotation() +
						cyPoint3f(PitchAmount * i_FrameTime, YawAmount * i_FrameTime, 0.f));

				}
			}
		}

		void TranslateBoundObject(GLFWwindow * window, float i_FrameTime)
		{
			assert(window);
			MousePosition MouseDisplacementInFrame = GetMouseDispaclementInFrame(window);
			
			int WindowWidth, WindowHeight;
			glfwGetWindowSize(window, &WindowWidth, &WindowHeight);
			float TranslateX = (float)MouseDisplacementInFrame.x/WindowWidth * MaxTranslateAmount * i_FrameTime;
			float TranslateY = (float)MouseDisplacementInFrame.y / WindowHeight * MaxTranslateAmount * i_FrameTime;

			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
			{
				//Translate Ctrl bound object
			}
			else if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
			{
				//Translate Alt bound object
			}
			else
			{
				//Camera zoom
				if (CameraGameObject)
				{
					CameraGameObject->SetPosition(CameraGameObject->GetPosition() + cyPoint3f(0.f, 0.f, TranslateY));
				}
			}
		}

		MousePosition GetMouseDispaclementInFrame(GLFWwindow * window)
		{
			assert(window);
			MousePosition CurrentMousePosition;
			glfwGetCursorPos(window, &CurrentMousePosition.x, &CurrentMousePosition.y);
			return MousePosition(CurrentMousePosition.x - LastMousePosition.x, CurrentMousePosition.y - LastMousePosition.y);
		}

		void SetCameraGameObject(Engine::Entity::GameObject * i_pGameObject)
		{
			assert(i_pGameObject);
			CameraGameObject = i_pGameObject;
		}

	}
}
