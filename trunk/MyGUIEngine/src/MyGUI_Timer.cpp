/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_Timer.h"

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
#	include <windows.h>
#	pragma comment(lib, "winmm.lib")
#else
#	include <sys/time.h>
#endif

namespace MyGUI
{

	Timer::Timer() :
		mTimeStart(0)
	{
	}

	void Timer::reset()
	{
		mTimeStart = getCurrentMilliseconds();
	}

	unsigned long Timer::getMilliseconds()
	{
		return getCurrentMilliseconds() - mTimeStart;
	}

	unsigned long Timer::getCurrentMilliseconds()
	{
#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
		/*
		We do this because clock() is not affected by timeBeginPeriod on Win32.
		QueryPerformanceCounter is a little overkill for the amount of precision that
		I consider acceptable. If someone submits a patch that replaces this code
		with QueryPerformanceCounter, I wouldn't complain. Until then, timeGetTime
		gets the results I'm after. -EMS

		See: http://www.geisswerks.com/ryan/FAQS/timing.html
		And: http://support.microsoft.com/default.aspx?scid=KB;EN-US;Q274323&
		*/
		return timeGetTime();
#else
		struct timeval now;
		gettimeofday(&now, NULL);
		return (now.tv_sec) * 1000 + (now.tv_usec) / 1000;
		//return ( unsigned long )(( double )( clock() ) / (( double )CLOCKS_PER_SEC / 1000.0 ) );
#endif
	}

} // namespace MyGUI
