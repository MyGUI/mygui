/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/

#ifndef CONTROLLER_RANDOM_PROGRESS_H_
#define CONTROLLER_RANDOM_PROGRESS_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ControllerItem.h"

namespace demo
{

	class ControllerRandomProgress :
		public MyGUI::ControllerItem
	{
		MYGUI_RTTI_DERIVED( ControllerRandomProgress )

	public:
		ControllerRandomProgress() : mTime(0) { }
		virtual ~ControllerRandomProgress() { }

		virtual bool addTime(MyGUI::Widget* _widget, float _time)
		{
			const float slice = 0.2;
			mTime += _time;
			if (mTime > slice)
			{
				mTime -= slice;
				action(_widget);
			}

			return true;
		}

		virtual void prepareItem(MyGUI::Widget* _widget)
		{
			mTime = 0;
			action(_widget);
		}

	private:
		void action(MyGUI::Widget* _widget)
		{
			if ((::rand() % 2) == 0)
			{
				MyGUI::ProgressBar* progress = _widget->castType<MyGUI::ProgressBar>(false);
				if (progress != nullptr)
				{
					size_t rande = progress->getProgressRange();
					if (rande != 0)
					{
						size_t position = progress->getProgressPosition();
						if (::rand() % 2)
						{
							if (position != 0)
								progress->setProgressPosition(position - 1);
						}
						else
						{
							if (position < rande)
								progress->setProgressPosition(position + 1);
						}
					}
				}
			}
		}

	private:
		float mTime;

	};

}

#endif // CONTROLLER_RANDOM_PROGRESS_H_
