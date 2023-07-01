/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/

#ifndef CONTROLLER_SMOOTH_CAPTION_H_
#define CONTROLLER_SMOOTH_CAPTION_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ControllerItem.h"

namespace demo
{

	class ControllerSmoothCaption : public MyGUI::ControllerItem
	{
		MYGUI_RTTI_DERIVED(ControllerSmoothCaption)

	public:
		bool addTime(MyGUI::Widget* _widget, float _time) override
		{
			const float slice = 0.04f;
			mTime += _time;
			if (mTime > slice)
			{
				mTime -= slice;
				update(_widget);
			}

			return true;
		}

		void prepareItem(MyGUI::Widget* _widget) override
		{
			mTime = 0;
			mCurrentPosition = 0;

			_widget->eventChangeProperty += MyGUI::newDelegate(this, &ControllerSmoothCaption::notifyChangeProperty);
		}

	private:
		void update(MyGUI::Widget* _widget)
		{
			if (mCurrentPosition < mNeedCaption.size())
			{
				mCurrentPosition++;
				MyGUI::TextBox* text = _widget->castType<MyGUI::TextBox>(false);
				if (text != nullptr)
				{
					if (mCurrentPosition == mNeedCaption.size())
						text->setCaption(mNeedCaption);
					else
						text->setCaption(mNeedCaption.substr(0, mCurrentPosition) + " _");
				}
			}
		}

		void notifyChangeProperty(MyGUI::Widget* _sender, std::string_view _key, std::string_view _value)
		{
			if (_key == "Caption")
			{
				mNeedCaption = _value;
				mCurrentPosition = 0;

				MyGUI::TextBox* text = _sender->castType<MyGUI::TextBox>(false);
				if (text != nullptr)
					text->setCaption(MyGUI::UString{});
			}
		}

	private:
		float mTime{0};
		std::string mNeedCaption;
		size_t mCurrentPosition{0};
	};

}

#endif // CONTROLLER_SMOOTH_CAPTION_H_
