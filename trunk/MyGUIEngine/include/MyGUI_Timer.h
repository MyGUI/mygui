/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_TIMER_H__
#define __MYGUI_TIMER_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	class MYGUI_EXPORT Timer
	{
	public:
		Timer();

		void reset();
		unsigned long getMilliseconds();

	private:
		unsigned long getCurrentMilliseconds();

	private:
		unsigned long mTimeStart;
	};

} // namespace MyGUI

#endif // __MYGUI_TIMER_H__
