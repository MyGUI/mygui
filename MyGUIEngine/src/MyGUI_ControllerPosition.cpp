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
#include "MyGUI_Precompiled.h"
#include "MyGUI_Prerequest.h"
#include "MyGUI_ControllerPosition.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Widget.h"
#include "MyGUI_ActionController.h"

namespace MyGUI
{

	ControllerPosition::ControllerPosition(const IntCoord & _destRect, float _time, MoveMode _mode) :
		mDestRect(_destRect), mTime(_time), mElapsedTime(0.), mCalcPosition(true), mCalcSize(true)
	{
		eventFrameAction = _getAction(_mode);
	}

	ControllerPosition::ControllerPosition(const IntSize & _destSize, float _time, MoveMode _mode) :
		mDestRect(IntPoint(), _destSize), mTime(_time), mElapsedTime(0.), mCalcPosition(false), mCalcSize(true)
	{
		eventFrameAction = _getAction(_mode);
	}

	ControllerPosition::ControllerPosition(const IntPoint & _destPoint, float _time, MoveMode _mode) :
		mDestRect(_destPoint, IntSize()), mTime(_time), mElapsedTime(0.), mCalcPosition(true), mCalcSize(false)
	{
		eventFrameAction = _getAction(_mode);
	}

	ControllerPosition::ControllerPosition(const IntCoord & _destRect, float _time, FrameAction::IDelegate * _action) :
		mDestRect(_destRect), mTime(_time), mElapsedTime(0.), mCalcPosition(true), mCalcSize(true)
	{
		eventFrameAction = _action;
	}

	ControllerPosition::ControllerPosition(const IntSize & _destSize, float _time, FrameAction::IDelegate * _action) :
		mDestRect(IntPoint(), _destSize), mTime(_time), mElapsedTime(0.), mCalcPosition(false), mCalcSize(true)
	{
		eventFrameAction = _action;
	}

	ControllerPosition::ControllerPosition(const IntPoint & _destPoint, float _time, FrameAction::IDelegate * _action) :
		mDestRect(_destPoint, IntSize()), mTime(_time), mElapsedTime(0.), mCalcPosition(true), mCalcSize(false)
	{
		eventFrameAction = _action;
	}

	const std::string & ControllerPosition::getType()
	{
		static std::string type("ControllerPositionController");
		return type;
	}

	void ControllerPosition::prepareItem(WidgetPtr _widget)
	{
		MYGUI_DEBUG_ASSERT(mTime > 0, "Time must be > 0");

		mStartRect = _widget->getCoord();

		// вызываем пользовательский делегат для подготовки
		eventPreAction(_widget);
	}

	bool ControllerPosition::addTime(WidgetPtr _widget, float _time)
	{
		mElapsedTime += _time;

		if (mElapsedTime < mTime)
		{
			IntCoord coord;
			eventFrameAction(mStartRect, mDestRect, coord, mElapsedTime/mTime);
			if (mCalcPosition) {
				if (mCalcSize) _widget->setCoord(coord);
				else _widget->setPosition(coord.point());
			}
			else if (mCalcSize) _widget->setSize(coord.size());

			// вызываем пользовательский делегат обновления
			eventUpdateAction(_widget);

			return true;
		}

		// поставить точно в конец
		IntCoord coord;
		eventFrameAction(mStartRect, mDestRect, coord, 1.0f);
		if (mCalcPosition) {
			if (mCalcSize) _widget->setCoord(coord);
			else _widget->setPosition(coord.point());
		}
		else if (mCalcSize) _widget->setSize(coord.size());

		// вызываем пользовательский делегат обновления
		eventUpdateAction(_widget);

		// вызываем пользовательский делегат пост обработки
		eventPostAction(_widget);

		return false;
	}

	ControllerPosition::FrameAction::IDelegate * ControllerPosition::_getAction(MoveMode _mode)
	{
		switch (_mode)
		{
			case Linear: return newDelegate(action::linearMoveFunction); break;
			case Accelerated: return newDelegate(action::acceleratedMoveFunction<30>); break;
			case Slowed: return newDelegate(action::acceleratedMoveFunction<4>); break;
			case Inertional: return newDelegate(action::inertionalMoveFunction); break;
		}
		MYGUI_DEBUG_ASSERT(false, "Invalid move mode");
		return nullptr;
	}

} // namespace MyGUI
