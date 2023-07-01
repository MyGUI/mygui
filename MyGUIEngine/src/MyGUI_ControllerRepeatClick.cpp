/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ControllerRepeatClick.h"

namespace MyGUI
{

	bool ControllerRepeatClick::addTime(MyGUI::Widget* _widget, float _time)
	{
		if (mTimeLeft == 0)
			mTimeLeft = mInit;

		if (mStep <= 0)
			return true;

		mTimeLeft -= _time;
		while (mTimeLeft <= 0)
		{
			mTimeLeft += mStep;
			eventRepeatClick(_widget, this);
		}
		return true;
	}

	void ControllerRepeatClick::setRepeat(float init, float step)
	{
		mInit = init;
		mStep = step;
	}

	void ControllerRepeatClick::setProperty(std::string_view _key, std::string_view _value)
	{
	}

	void ControllerRepeatClick::prepareItem(MyGUI::Widget* _widget)
	{
	}
}
