/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/

#ifndef CONTROLLER_SMOOTH_PROGRESS_H_
#define CONTROLLER_SMOOTH_PROGRESS_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ControllerItem.h"

namespace demo
{

	class ControllerSmoothProgress :
		public MyGUI::ControllerItem
	{
		MYGUI_RTTI_DERIVED( ControllerSmoothProgress )

	public:
		ControllerSmoothProgress() :
			mTime(0),
			mCurrentPosition(0),
			mNeedPosition(0)
		{ }
		virtual ~ControllerSmoothProgress() { }

		virtual bool addTime(MyGUI::Widget* _widget, float _time)
		{
			const float slice = 0.04;
			mTime += _time;
			if (mTime > slice)
			{
				mTime -= slice;
				update(_widget);
			}

			return true;
		}

		virtual void prepareItem(MyGUI::Widget* _widget)
		{
			mTime = 0;
			mCurrentPosition = 0;
			mNeedPosition = 0;

			MyGUI::ProgressBar* progress = _widget->castType<MyGUI::ProgressBar>(false);
			if (progress != nullptr)
			{
				_widget->eventChangeProperty += MyGUI::newDelegate(this, &ControllerSmoothProgress::notifyChangeProperty);
			}
		}

	private:
		void update(MyGUI::Widget* _widget)
		{
			if (mCurrentPosition != mNeedPosition)
			{
				MyGUI::ProgressBar* progress = _widget->castType<MyGUI::ProgressBar>(false);
				if (progress != nullptr)
				{
					if (mCurrentPosition > mNeedPosition)
					{
						mCurrentPosition -= 1;
						progress->setProgressPosition(mCurrentPosition);
					}
					else
					{
						mCurrentPosition += 1;
						progress->setProgressPosition(mCurrentPosition);
					}
				}
			}
		}

		void notifyChangeProperty(MyGUI::Widget* _sender, const std::string& _key, const std::string& _value)
		{
			if (_key == "RangePosition")
			{
				MyGUI::ProgressBar* progress = _sender->castType<MyGUI::ProgressBar>(false);
				if (progress != nullptr)
				{
					progress->setProgressPosition(mCurrentPosition);
				}
				mNeedPosition = MyGUI::utility::parseValue<size_t>(_value);
			}
		}

	private:
		float mTime;
		size_t mCurrentPosition;
		size_t mNeedPosition;

	};

}

#endif // CONTROLLER_SMOOTH_PROGRESS_H_
