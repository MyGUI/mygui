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

	/** This controller used for smooth changing position of widget in time */
	class MYGUI_EXPORT ControllerPosition : public ControllerItem
	{
		typedef delegates::CDelegate4<const IntCoord&, const IntCoord&, IntCoord&, float> FrameAction;
		typedef delegates::CDelegate4<const IntCoord&, const IntCoord&, IntCoord&, float> FrameAction;
	public:
		enum MoveMode{
			Linear, //!< Constant speed
			Accelerated, //!< Start with zero speed, increasing all time
			Slowed, //!< Start with maximum speed, decreasing to zero at the end
			Inertional, //!< Start with zero speed increasing half time and then decreasing to zero
			Jump //!< Start with zero speed increasing half time and then decreasing to zero
		};

		/**
			@param _destRect destination coordinate
			@param _time seconds in which widget will reach destination coordinate
			@param _mode of moving (see ControllerPosition::MoveMode)
		*/
		ControllerPosition(const IntCoord & _destRect, float _time, MoveMode _mode);
		//! @copydoc ControllerPosition(const IntCoord & _destRect, float _time, MoveMode _mode)
		ControllerPosition(const IntSize & _destSize, float _time, MoveMode _mode);
		//! @copydoc ControllerPosition(const IntCoord & _destRect, float _time, MoveMode _mode)
		ControllerPosition(const IntPoint & _destPoint, float _time, MoveMode _mode);
		/**
			@param _destRect destination coordinate
			@param _time seconds in which widget planned to reach destination coordinate
			@param _action applied to widget every frame (see ControllerPosition::eventFrameAction)
		*/
		ControllerPosition(const IntCoord & _destRect, float _time, FrameAction::IDelegate * _action);
		ControllerPosition(const IntSize & _destSize, float _time, FrameAction::IDelegate * _action);
		ControllerPosition(const IntPoint & _destPoint, float _time, FrameAction::IDelegate * _action);

	private:

		const std::string & getType();
		bool addTime(WidgetPtr _widget, float _time);
		void prepareItem(WidgetPtr _widget);

		float getElapsedTime() { return mElapsedTime; }

		IntCoord mStartRect;
		IntCoord mDestRect;
		float mTime;
		float mElapsedTime;

		// controller changing position
		bool mCalcPosition;
		// controller changing size
		bool mCalcSize;

		/** Event : Every frame action while controller exist.\n
			signature : void method(const IntRect & _startRect, const IntRect & _destRect, IntRect & _result, float _current_time)\n
			@param _startRect start coordinate of widget
			@param _destRect destination coordinate
			@param _result resultRect
			@param _current_time elapsed time (_current_time is real elapsed time divided by _time(see constructor) so _current_time == 1 mean that elapsed _time seconds)
		*/
		FrameAction eventFrameAction;
	};

}

#endif // __MYGUI_CONTROLLER_POSITION_H__
