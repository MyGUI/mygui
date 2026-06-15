/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_CONTROLLER_REPEATCLICK_H_
#define MYGUI_CONTROLLER_REPEATCLICK_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Delegate.h"
#include "MyGUI_Types.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ControllerItem.h"


namespace MyGUI
{

	/** This controller can be used to repeatedly trigger an event at specific time steps. */
	class MYGUI_EXPORT ControllerRepeatClick : public ControllerItem
	{
		MYGUI_RTTI_DERIVED(ControllerRepeatClick)

	public:
		/** Change initial delay and time step
			@param init The delay before the first event will be triggered.
			@param step The delay after each event before the next event is triggered.
		 */
		void setRepeat(float init, float step);

		bool addTime(Widget* _widget, float _time) override;
		void prepareItem(Widget* _widget) override;
		void setProperty(std::string_view _key, std::string_view _value) override;

		using EventHandle_RepeatClick = delegates::MultiDelegate<Widget*, ControllerItem*>;

		/** Event : Repeat Click.\n
			signature : void method(MyGUI::Widget* _sender, MyGUI::ControllerItem *_controller)\n
		*/
		EventHandle_RepeatClick eventRepeatClick;

	private:
		float mInit{0.5f};
		float mStep{0.1f};
		float mTimeLeft{0};
	};

}

#endif
