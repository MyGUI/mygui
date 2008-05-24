/*!
	@file
	@author		Evmenov Georgiy
	@date		03/2008
	@module
*/
#ifndef __MYGUI_CONTROLLER_POSITION_H__
#define __MYGUI_CONTROLLER_POSITION_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ControllerItem.h"

namespace MyGUI
{

	class _MyGUIExport ControllerPosition : public ControllerItem
	{
		typedef delegates::CDelegate4<WidgetPtr, IntCoord&, IntCoord&, float> FrameAction;
	public:
		enum MoveMode{
			Linear, //!< Constant speed
			Accelerated, //!< Start with zero speed, increasing all time
			Slowed, //!< Start with maximum speed, decreasing to zero at the end
			Inertional //!< Start with zero speed increasing half time and then decreasing to zero
		};

		/** 
			@param _destRect destination coordinate
			@param _time seconds in which widget will reach destination coordinate
			@param _mode of moving (see ControllerPosition::MoveMode)
		*/
		ControllerPosition(IntCoord _destRect, float _time, MoveMode _mode);
		/** 
			@param _destRect destination coordinate
			@param _time seconds in which widget planned to reach destination coordinate
			@param _action applied to widget every frame (see ControllerPosition::eventFrameAction)
		*/
		ControllerPosition(IntCoord _destRect, float _time, FrameAction _action);

	private:
		/** Event : Every frame action while controller exist.\n
			signature : void method(MyGUI::WidgetPtr _widget, IntRect & _startRect, IntRect & _destRect, float _current_time)\n
			@param _widget - controlled widget
			@param _startRect start coordinate of widget
			@param _destRect destination coordinate
			@param _current_time elapsed time (_current_time is real elapsed time divided by _time(see constructor) so _current_time == 1 mean that elapsed _time seconds)
		*/
		FrameAction eventFrameAction;

		const std::string & getType();
		bool addTime(WidgetPtr _widget, float _time);
		void prepareItem(WidgetPtr _widget);
		void replaseItem(WidgetPtr _widget, ControllerItem * _item);

		inline float getElapsedTime() {return mElapsedTime;}

		IntCoord mStartRect;
		IntCoord mDestRect;
		float mTime;
		float mElapsedTime;
	};

}

#endif // __MYGUI_CONTROLLER_POSITION_H__
