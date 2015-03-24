/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_CONTROLLER_POSITION_H_
#define MYGUI_CONTROLLER_POSITION_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Delegate.h"
#include "MyGUI_Types.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ControllerItem.h"

namespace MyGUI
{

	/** This controller used for smooth changing position of widget in time */
	class MYGUI_EXPORT ControllerPosition :
		public ControllerItem
	{
		MYGUI_RTTI_DERIVED( ControllerPosition )

	public:
		typedef delegates::CDelegate4<const IntCoord&, const IntCoord&, IntCoord&, float> FrameAction;

		ControllerPosition();
		virtual ~ControllerPosition();

		void setCoord(const IntCoord& _value);

		void setSize(const IntSize& _value);

		void setPosition(const IntPoint& _value);

		void setFunction(const std::string& _value);

		/**
			@param _value seconds in which widget planned to reach destination coordinate
		*/
		void setTime(float _value);

		/**
			@param _value Delegate applied to widget every frame (see ControllerPosition::eventFrameAction)
		*/
		void setAction(FrameAction::IDelegate* _value);

		virtual bool addTime(Widget* _widget, float _time);
		virtual void prepareItem(Widget* _widget);
		virtual void setProperty(const std::string& _key, const std::string& _value);

	private:
		IntCoord mStartCoord;
		IntCoord mDestCoord;
		float mTime;
		float mElapsedTime;

		// controller changing position
		bool mCalcPosition;
		// controller changing size
		bool mCalcSize;

		/** Event : Every frame action while controller exist.\n
			signature : void method(const IntRect& _startRect, const IntRect& _destRect, IntRect& _result, float _current_time)\n
			@param _startRect start coordinate of widget
			@param _destRect destination coordinate
			@param _result resultRect
			@param _current_time elapsed time (_current_time is real elapsed time divided by _time(see constructor) so _current_time == 1 mean that _time seconds elapsed)
		*/
		FrameAction eventFrameAction;
	};

} // namespace MyGUI

#endif // MYGUI_CONTROLLER_POSITION_H_
