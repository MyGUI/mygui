/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_ControllerRepeatClick.h"

namespace MyGUI
{
	ControllerRepeatClick::ControllerRepeatClick() :
		mInit(0.5),
		mStep(0.1),
		mTimeLeft(0)
	{
	}

	ControllerRepeatClick::~ControllerRepeatClick()
	{
	}

	bool ControllerRepeatClick::addTime(MyGUI::Widget* _widget, float _time)
	{
		if(mTimeLeft == 0)
			mTimeLeft = mInit;

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

	void ControllerRepeatClick::setProperty(const std::string& _key, const std::string& _value)
	{
	}

	void ControllerRepeatClick::prepareItem(MyGUI::Widget* _widget)
	{
	}
}

