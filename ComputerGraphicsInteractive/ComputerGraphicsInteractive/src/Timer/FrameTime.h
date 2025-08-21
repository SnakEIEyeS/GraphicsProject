/* FrameTime calculation from a previous project*/

#pragma once

#include <windows.h>

#define DESIRED_FPS				60.0f
#define DESIRED_FRAMETIME_MS	(1000.0f / DESIRED_FPS)
#define MAX_FRAMETIME_MS		(2 * DESIRED_FRAMETIME_MS)


namespace Engine
{
	namespace Timing
	{
		typedef __int64 tick;

		void CalcCPUFrequency();

		float CalcLastFrameTime_ms();

		float GetLastFrameTime_ms();

		tick GetCurrentTickCounter();
		tick GetTickDiff(tick i_StartTick);

		float GetTimeDiff_ms(tick i_StartTick);
	}
}
