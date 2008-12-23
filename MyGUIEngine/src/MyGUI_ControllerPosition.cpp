/*!
	@file
	@author		Evmenov Georgiy
	@date		03/2008
	@module
*/
#include "MyGUI_Prerequest.h"
#include "MyGUI_ControllerPosition.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{


	inline void MoveFunction(const IntCoord & _startRect, const IntCoord & _destRect, IntCoord & _result, float _k){
		_result.set(_startRect.left   - int( float(_startRect.left   - _destRect.left)   * _k ),
					_startRect.top    - int( float(_startRect.top    - _destRect.top)    * _k ),
					_startRect.width  - int( float(_startRect.width  - _destRect.width)  * _k ),
					_startRect.height - int( float(_startRect.height - _destRect.height) * _k )
		);
	};

	void LinearMoveFunction(const IntCoord & _startRect, const IntCoord & _destRect, IntCoord & _result, float _current_time)
	{
		MoveFunction(_startRect, _destRect, _result, _current_time);
	}

	template <int N>
	void AcceleratedMoveFunction(const IntCoord & _startRect, const IntCoord & _destRect, IntCoord & _result, float _current_time)
	{
		float k = pow (_current_time, N/10.f /*3 by default as Accelerated and 0.4 by default as Slowed*/);
		MoveFunction(_startRect, _destRect, _result, k);
	}

	template <int N>
	void JumpFunction(const IntCoord & _startRect, const IntCoord & _destRect, IntCoord & _result, float _current_time)
	{
		float k = pow (_current_time, 2) * (-2 - N/10.f) + _current_time * (3 + N/10.f);
		MoveFunction(_startRect, _destRect, _result, k);
	}

	void InertionalMoveFunction(const IntCoord & _startRect, const IntCoord & _destRect, IntCoord & _result, float _current_time)
	{
		#ifndef M_PI
		const float M_PI = 3.141593;
		#endif
		float k = sin(M_PI * _current_time - M_PI/2);
		if (k<0) k = (-pow((-k), (float)0.7) + 1)/2;
		else k = (pow((k), (float)0.7) + 1)/2;
		MoveFunction(_startRect, _destRect, _result, k);
	}

	ControllerPosition::ControllerPosition(const IntCoord & _destRect, float _time, MoveMode _mode) :
		mDestRect(_destRect), mTime(_time), mElapsedTime(0.), mCalcPosition(true), mCalcSize(true)
	{
		switch (_mode)
		{
			case Linear: eventFrameAction = newDelegate(LinearMoveFunction); break;
			case Accelerated: eventFrameAction = newDelegate(AcceleratedMoveFunction<30>); break;
			case Slowed: eventFrameAction = newDelegate(AcceleratedMoveFunction<4>); break;
			case Inertional: eventFrameAction = newDelegate(InertionalMoveFunction); break;
		}
	}

	ControllerPosition::ControllerPosition(const IntSize & _destSize, float _time, MoveMode _mode) :
		mDestRect(IntPoint(), _destSize), mTime(_time), mElapsedTime(0.), mCalcPosition(false), mCalcSize(true)
	{
		switch (_mode)
		{
			case Linear: eventFrameAction = newDelegate(LinearMoveFunction); break;
			case Accelerated: eventFrameAction = newDelegate(AcceleratedMoveFunction<30>); break;
			case Slowed: eventFrameAction = newDelegate(AcceleratedMoveFunction<4>); break;
			case Inertional: eventFrameAction = newDelegate(InertionalMoveFunction); break;
		}
	}

	ControllerPosition::ControllerPosition(const IntPoint & _destPoint, float _time, MoveMode _mode) :
		mDestRect(_destPoint, IntSize()), mTime(_time), mElapsedTime(0.), mCalcPosition(true), mCalcSize(false)
	{
		switch (_mode)
		{
			case Linear: eventFrameAction = newDelegate(LinearMoveFunction); break;
			case Accelerated: eventFrameAction = newDelegate(AcceleratedMoveFunction<30>); break;
			case Slowed: eventFrameAction = newDelegate(AcceleratedMoveFunction<4>); break;
			case Inertional: eventFrameAction = newDelegate(InertionalMoveFunction); break;
		}
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

} // namespace MyGUI
