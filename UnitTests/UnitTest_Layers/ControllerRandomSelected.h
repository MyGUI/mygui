/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/

#ifndef CONTROLLER_RANDOM_SELECTED_H_
#define CONTROLLER_RANDOM_SELECTED_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ControllerItem.h"

namespace demo
{

	class ControllerRandomSelected : public MyGUI::ControllerItem
	{
		MYGUI_RTTI_DERIVED(ControllerRandomSelected)

	public:
		bool addTime(MyGUI::Widget* _widget, float _time) override
		{
			mTime += _time;
			if (mTime > 1)
			{
				mTime -= 1;
				action(_widget);
			}

			return true;
		}

		void prepareItem(MyGUI::Widget* _widget) override
		{
			mTime = 0;
			action(_widget);
		}

	private:
		void action(MyGUI::Widget* _widget)
		{
			if ((::rand() % 3) == 0)
			{
				MyGUI::Button* button = _widget->castType<MyGUI::Button>(false);
				if (button != nullptr)
					button->setStateSelected(!button->getStateSelected());
			}
		}

	private:
		float mTime{0};
	};

}

#endif // CONTROLLER_RANDOM_SELECTED_H_
