/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_TIMER_H_
#define MYGUI_TIMER_H_

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

#endif // MYGUI_TIMER_H_
