#include "Input.h"

#include <assert.h>
#include <iostream>

#include <glad/glad.h>
#include <GL/glfw3.h>

#include "../cyCodeBase/cyPoint.h"
#include "../GameObject/GameObject.h"

namespace Engine
{
	namespace Input
	{
		MousePosition LastMousePosition;
		Engine::Entity::GameObject* CameraGameObject = nullptr;
		Engine::Entity::GameObject* CtrlBoundGameObject = nullptr;
		Engine::Entity::GameObject* AltBoundGameObject = nullptr;
		Engine::Entity::GameObject* ShiftBoundGameObject = nullptr;

		//Debug Draw variables
		bool AlreadyDown_Space = false;
		bool DebugDrawRequested = true;

		//Tessellation variables;
		GLint TessellationLevelMax;
		float TessellationLevelMin = 5.f;
		float TessellationLevelCurrent = TessellationLevelMin;
		float TessellationLevelDelta = 1.f;
		bool AlreadyDown_LeftArrow = false;
		bool AlreadyDown_RightArrow = false;

		//MLAA variables
		bool MLAA_Enabled = true;
		bool MLAA_BlendingWeightsPass = true;
		bool MLAA_NeighborBlendingPass = true;
		bool AlreadyDown_R = false;
		bool AlreadyDown_1 = false;
		bool AlreadyDown_2 = false;
		bool AlreadyDown_3 = false;

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

			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				if (!AlreadyDown_Space)
				{
					AlreadyDown_Space = true;
					DebugDrawRequested = !DebugDrawRequested;
				}
			}
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
			{
				if (AlreadyDown_Space)
				{
					AlreadyDown_Space = false;
				}
				
			}

			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				if (!AlreadyDown_LeftArrow)
				{
					AlreadyDown_LeftArrow = true;
					UpdateTessellationLevel(-TessellationLevelDelta);
				}
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE)
			{
				if (AlreadyDown_LeftArrow)
				{
					AlreadyDown_LeftArrow = false;
				}
			}

			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				if (!AlreadyDown_RightArrow)
				{
					AlreadyDown_RightArrow = true;
					UpdateTessellationLevel(TessellationLevelDelta);
				}
			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
			{
				if (AlreadyDown_RightArrow)
				{
					AlreadyDown_RightArrow = false;
				}
			}

			HandleMLAAControls(window);
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
				if (CtrlBoundGameObject)
				{
					CtrlBoundGameObject->SetRotation(CtrlBoundGameObject->GetRotation() + cyPoint3f(PitchAmount, YawAmount, 0.f));
				}
			}
			else if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
			{
				//Rotate Alt bound object
				if (AltBoundGameObject)
				{
					AltBoundGameObject->SetRotation(AltBoundGameObject->GetRotation() + cyPoint3f(PitchAmount, YawAmount, 0.f));
				}
			}
			else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			{
				//Rotate Shift bound object
				if (ShiftBoundGameObject)
				{
					ShiftBoundGameObject->SetRotation(ShiftBoundGameObject->GetRotation() + cyPoint3f(PitchAmount, YawAmount, 0.f));
				}
			}
			else
			{
				//Rotate Camera
				if (CameraGameObject)
				{
					CameraGameObject->SetRotation(CameraGameObject->GetRotation() + cyPoint3f(PitchAmount, YawAmount, 0.f));

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
				if (CtrlBoundGameObject)
				{
					//Negate Y translation because TopLeft of window is (0,0) and BottomRight is (Width, Height)
					CtrlBoundGameObject->SetPosition(CtrlBoundGameObject->GetPosition() + cyPoint3f(TranslateX, -TranslateY, 0.f));
				}
				//Negate Y translation because TopLeft of window is (0,0) and BottomRight is (Width, Height)
			}
			else if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
			{
				//Translate Alt bound object
				if (AltBoundGameObject)
				{
					//Negate Y translation because TopLeft of window is (0,0) and BottomRight is (Width, Height)
					AltBoundGameObject->SetPosition(AltBoundGameObject->GetPosition() + cyPoint3f(TranslateX, -TranslateY, 0.f));
				}
			}
			else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			{
				//Translate Shift bound object
				if (ShiftBoundGameObject)
				{
					//Negate Y translation because TopLeft of window is (0,0) and BottomRight is (Width, Height)
					ShiftBoundGameObject->SetPosition(ShiftBoundGameObject->GetPosition() + cyPoint3f(TranslateX, -TranslateY, 0.f));
				}
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

		void SetCtrlBoundGameObject(Engine::Entity::GameObject * i_pGameObject)
		{
			assert(i_pGameObject);
			CtrlBoundGameObject = i_pGameObject;
		}

		void SetAltBoundGameObject(Engine::Entity::GameObject * i_pGameObject)
		{
			assert(i_pGameObject);
			AltBoundGameObject = i_pGameObject;
		}

		void SetShiftBoundGameObject(Engine::Entity::GameObject * i_pGameObject)
		{
			assert(i_pGameObject);
			ShiftBoundGameObject = i_pGameObject;
		}

		bool IsDebugDrawRequested()
		{
			return DebugDrawRequested;
		}

		void SetupMaxTessellationLevel()
		{
			glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &TessellationLevelMax);
			TessellationLevelCurrent = (float)TessellationLevelMax;
		}

		float GetTessellationLevel()
		{
			return TessellationLevelCurrent;
		}

		void UpdateTessellationLevel(float i_DeltaTessLevel)
		{
			TessellationLevelCurrent += i_DeltaTessLevel;
			if (TessellationLevelCurrent < TessellationLevelMin)
			{
				TessellationLevelCurrent = TessellationLevelMin;
			}
			else if (TessellationLevelCurrent > TessellationLevelMax)
			{
				TessellationLevelCurrent = (float)TessellationLevelMax;
			}
		}

		bool GetMLAAEnabled()
		{
			return MLAA_Enabled;
		}

		bool GetMLAABlendingWeightsPassEnabled()
		{
			return MLAA_BlendingWeightsPass;
		}

		bool GetMLAANeighborBlendPassEnabled()
		{
			return MLAA_NeighborBlendingPass;
		}

		void HandleMLAAControls(GLFWwindow * window)
		{
			/*************************** MLAA Controls *****************************/
			//MLAA On/Off
			if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
			{
				if (!AlreadyDown_R)
				{
					AlreadyDown_R = true;
					//Set MLAA On/Off
					MLAA_Enabled = !MLAA_Enabled;
				}
			}
			if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
			{
				if (AlreadyDown_R)
				{
					AlreadyDown_R = false;
				}
			}

			//MLAA Passes
			//Edge Detection Pass
			if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			{
				if (!AlreadyDown_1)
				{
					AlreadyDown_1 = true;
					if (MLAA_Enabled)
					{
						ChooseMLAAPass(1);
					}
				}
			}
			if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE)
			{
				if (AlreadyDown_1)
				{
					AlreadyDown_1 = false;
				}
			}

			//Blending Weights Pass
			if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			{
				if (!AlreadyDown_2)
				{
					AlreadyDown_2 = true;
					if (MLAA_Enabled)
					{
						ChooseMLAAPass(2);

					}
				}
			}
			if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE)
			{
				if (AlreadyDown_2)
				{
					AlreadyDown_2 = false;
				}
			}

			//Neighbor Blending Pass
			if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
			{
				if (!AlreadyDown_3)
				{
					AlreadyDown_3 = true;
					if (MLAA_Enabled)
					{
						ChooseMLAAPass(3);

					}
				}
			}
			if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE)
			{
				if (AlreadyDown_3)
				{
					AlreadyDown_3 = false;
				}
			}
		}

		void ChooseMLAAPass(unsigned int i_MLAAPassNumber)
		{
			switch (i_MLAAPassNumber)
			{
			case 1:
				//Display Edge Detection Pass
				MLAA_BlendingWeightsPass = false;
				MLAA_NeighborBlendingPass = false;
				break;

			case 2:
				//Display Blending Weights Pass
				MLAA_BlendingWeightsPass = true;
				MLAA_NeighborBlendingPass = false;
				break;

			case 3:
				//Display Neighbor Blending Pass
				MLAA_BlendingWeightsPass = true;
				MLAA_NeighborBlendingPass = true;
				break;

			default:
				std::cout << "Received invalid MLAA Pass number\n";
				break;
			}
		}

	}
}
