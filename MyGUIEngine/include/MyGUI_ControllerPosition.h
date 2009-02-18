/*!
	@file
	@author		Evmenov Georgiy
	@date		03/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
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
	public:
		typedef delegates::CDelegate4<const IntCoord&, const IntCoord&, IntCoord&, float> FrameAction;

	public:
		typedef enum /*MYGUI_OBSOLETE_START("use : actions from MyGUI::action")*/
		{
			// OBSOLETE, use MyGUI::newDelegate(action::linearMoveFunction) instead
			Linear, //!< Constant speed
			// OBSOLETE, use MyGUI::newDelegate(action::acceleratedMoveFunction<30>) instead
			Accelerated, //!< Start with zero speed, increasing all time
			// OBSOLETE, use MyGUI::newDelegate(action::acceleratedMoveFunction<4>) instead
			Slowed, //!< Start with maximum speed, decreasing to zero at the end
			// OBSOLETE, use MyGUI::newDelegate(action::inertionalMoveFunction) instead
			Inertional //!< Start with zero speed increasing half time and then decreasing to zero
		} /*MYGUI_OBSOLETE_END*/ MoveMode;

		/**
			@param _destRect destination coordinate
			@param _time seconds in which widget will reach destination coordinate
			@param _mode of moving (see ControllerPosition::MoveMode)
		*/
		MYGUI_OBSOLETE("use : ControllerPosition(const IntCoord & _destRect, float _time, FrameAction::IDelegate * _action)")
		ControllerPosition(const IntCoord & _destRect, float _time, MoveMode _mode);
		//! @copydoc ControllerPosition(const IntCoord & _destRect, float _time, MoveMode _mode)
		MYGUI_OBSOLETE("use : ControllerPosition(const IntSize & _destSize, float _time, FrameAction::IDelegate * _action)")
		ControllerPosition(const IntSize & _destSize, float _time, MoveMode _mode);
		//! @copydoc ControllerPosition(const IntCoord & _destRect, float _time, MoveMode _mode)
		MYGUI_OBSOLETE("use : ControllerPosition(const IntPoint & _destPoint, float _time, FrameAction::IDelegate * _action)")
		ControllerPosition(const IntPoint & _destPoint, float _time, MoveMode _mode);
		/**
			@param _destRect destination coordinate
			@param _time seconds in which widget planned to reach destination coordinate
			@param _action applied to widget every frame (see ControllerPosition::eventFrameAction)
		*/
		ControllerPosition(const IntCoord & _destRect, float _time, FrameAction::IDelegate * _action);
		//! @copydoc ControllerPosition(const IntCoord & _destRect, FrameAction::IDelegate * _action)
		ControllerPosition(const IntSize & _destSize, float _time, FrameAction::IDelegate * _action);
		//! @copydoc ControllerPosition(const IntCoord & _destRect, FrameAction::IDelegate * _action)
		ControllerPosition(const IntPoint & _destPoint, float _time, FrameAction::IDelegate * _action);

	private:

		const std::string & getType();
		bool addTime(WidgetPtr _widget, float _time);
		void prepareItem(WidgetPtr _widget);

		float getElapsedTime() { return mElapsedTime; }

		FrameAction::IDelegate * _getAction(MoveMode _mode);

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
			@param _current_time elapsed time (_current_time is real elapsed time divided by _time(see constructor) so _current_time == 1 mean that _time seconds elapsed)
		*/
		FrameAction eventFrameAction;
	};

}

#endif // __MYGUI_CONTROLLER_POSITION_H__
