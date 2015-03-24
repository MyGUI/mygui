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
	class ControllerRepeatClick :
		public ControllerItem
	{
		MYGUI_RTTI_DERIVED( ControllerRepeatClick )

	public:
		ControllerRepeatClick();
		virtual ~ControllerRepeatClick();

		/** Change initial delay and time step
			@param init The delay before the first event will be triggered.
			@param step The delay after each event before the next event is triggered.
		 */
		void setRepeat(float init, float step);

		virtual bool addTime(Widget* _widget, float _time);
		virtual void prepareItem(Widget* _widget);
		virtual void setProperty(const std::string& _key, const std::string& _value);

		typedef delegates::CMultiDelegate2<Widget*, ControllerItem*> EventHandle_RepeatClick;

		/** Event : Repeat Click.\n
			signature : void method(MyGUI::Widget* _sender, MyGUI::ControllerItem *_controller)\n
		*/
		EventHandle_RepeatClick eventRepeatClick;

	private:
		float mInit;
		float mStep;
		float mTimeLeft;
	};

}

#endif
