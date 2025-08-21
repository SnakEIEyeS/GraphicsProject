/* FrameTime calculation from a previous project*/

#include "FrameTime.h"

#include <iostream>

float g_CPUFrequency = 0.0f;
__int64 g_LastFrameStartTick = 0;

namespace Engine
{
	namespace Timing
	{
		void CalcCPUFrequency()
		{
			LARGE_INTEGER li;
			if (!QueryPerformanceFrequency(&li))
			{
#ifdef DEBUG
				std::cout << "Failed to get CPU Frequency.\n";
#endif // DEBUG			
			}

			g_CPUFrequency = float(li.QuadPart) / 1000.0f;
		}

		float CalcLastFrameTime_ms()
		{
			float LastFrameTime_ms;
			LARGE_INTEGER li;

			QueryPerformanceCounter(&li);
			__int64 currentTick = li.QuadPart;

			if (g_LastFrameStartTick)
			{
				__int64 elapsedTicks = (currentTick - g_LastFrameStartTick);

				LastFrameTime_ms = elapsedTicks / g_CPUFrequency;
			}
			else
			{
				LastFrameTime_ms = 13.3f;
			}

			g_LastFrameStartTick = currentTick;

			return LastFrameTime_ms;
		}

		float GetLastFrameTime_ms()
		{
#if defined( CONSTANT_FRAMETIME )
			return DESIRED_FRAMETIME_MS;
#elif defined( CLAMP_FRAMETIME )
			float LastFrameTime_ms = CalcLastFrameTime_ms();
			if (LastFrameTime_ms > MAX_FRAMETIME_MS)
				return MAX_FRAMETIME_MS;
			else
				return LastFrameTime_ms;
#else
			return CalcLastFrameTime_ms();
#endif // ( CONSTANT_FRAMETIME )

		}

		tick GetCurrentTickCounter()
		{
			LARGE_INTEGER li;

			QueryPerformanceCounter(&li);
			__int64 currentTick = li.QuadPart;

			return currentTick;
		}

		tick GetTickDiff(tick i_StartTick)
		{
			LARGE_INTEGER li;

			QueryPerformanceCounter(&li);
			__int64 currentTick = li.QuadPart;

			return (currentTick - i_StartTick);
		}

		float GetTimeDiff_ms(tick i_StartTick)
		{
			LARGE_INTEGER li;

			QueryPerformanceCounter(&li);
			__int64 currentTick = li.QuadPart;

			float TimeDiff_ms = (currentTick - i_StartTick) / g_CPUFrequency;

			return TimeDiff_ms;
		}
	}
}

