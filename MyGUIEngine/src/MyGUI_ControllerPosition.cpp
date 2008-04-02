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
	void LinearMoveFunction(MyGUI::WidgetPtr _widget, IntCoord & _startRect, IntCoord & _destRect, float _current_time)
	{
		float k = _current_time;
		IntCoord rect(_startRect.left - (_startRect.left - _destRect.left) * k,
			            _startRect.top - (_startRect.top - _destRect.top) * k,
								  _startRect.width - (_startRect.width - _destRect.width) * k,
								  _startRect.height - (_startRect.height - _destRect.height) * k);
		_widget->setPosition(rect);
	}

	void AcceleratedMoveFunction(MyGUI::WidgetPtr _widget, IntCoord & _startRect, IntCoord & _destRect, float _current_time)
	{
		float k = pow (_current_time, (float)3);
		IntCoord rect(_startRect.left - (_startRect.left - _destRect.left) * k,
			            _startRect.top - (_startRect.top - _destRect.top) * k,
								  _startRect.width - (_startRect.width - _destRect.width) * k,
								  _startRect.height - (_startRect.height - _destRect.height) * k);
		_widget->setPosition(rect);
	}

	void SlowedMoveFunction(MyGUI::WidgetPtr _widget, IntCoord & _startRect, IntCoord & _destRect, float _current_time)
	{
		float k = pow (_current_time, (float)0.4);
		IntCoord rect(_startRect.left - (_startRect.left - _destRect.left) * k,
			            _startRect.top - (_startRect.top - _destRect.top) * k,
								  _startRect.width - (_startRect.width - _destRect.width) * k,
								  _startRect.height - (_startRect.height - _destRect.height) * k);
		_widget->setPosition(rect);
	}

	void InertionalMoveFunction(MyGUI::WidgetPtr _widget, IntCoord & _startRect, IntCoord & _destRect, float _current_time)
	{
		const float M_PI = 3.14;
		float k = sin(M_PI * _current_time - M_PI/2);
		if (k<0) k = (-pow((-k), 2) + 1)/2;
		else k = (pow((k), 2) + 1)/2;
		IntCoord rect(_startRect.left - (_startRect.left - _destRect.left) * k,
			            _startRect.top - (_startRect.top - _destRect.top) * k,
								  _startRect.width - (_startRect.width - _destRect.width) * k,
								  _startRect.height - (_startRect.height - _destRect.height) * k);
		_widget->setPosition(rect);
	}

	ControllerPosition::ControllerPosition(IntCoord _destRect, float _time, MoveMode _mode):
		mDestRect(_destRect), mTime(_time), mElapsedTime(0.)
	{
		switch (_mode)
		{
		case Linear: eventFrameAction = newDelegate(LinearMoveFunction); break;
		case Accelerated: eventFrameAction = newDelegate(AcceleratedMoveFunction); break;
		case Slowed: eventFrameAction = newDelegate(SlowedMoveFunction); break;
		case Inertional: eventFrameAction = newDelegate(InertionalMoveFunction); break;
		}
	}

	ControllerPosition::ControllerPosition(IntCoord _destRect, float _time, FrameAction _action):
		mDestRect(_destRect), mTime(_time), eventFrameAction(_action), mElapsedTime(0.)
	{
	}

	const std::string & ControllerPosition::getType()
	{
		static std::string type("ControllerPositionController");
		return type;
	}

	void ControllerPosition::prepareItem(WidgetPtr _widget)
	{
		MYGUI_DEBUG_ASSERT(mSpeed > 0, "coef must be > 0");

		mStartRect = _widget->getCoord();

		// вызываем пользовательский делегат для подготовки
		eventPreAction(_widget);
	}

	void ControllerPosition::replaseItem(WidgetPtr _widget, ControllerItem * _item)
	{
		ControllerPosition * item = static_cast<ControllerPosition*>(_item);
	}

	bool ControllerPosition::addTime(WidgetPtr _widget, float _time)
	{
		mElapsedTime += _time;

		//if (
			eventFrameAction(_widget, mStartRect, mDestRect, mElapsedTime/mTime);
		//	)
		if (mElapsedTime < mTime)	return true;

		// вызываем пользовательский елегат пост обработки
		eventPostAction(_widget);

		return false;
	}

} // namespace MyGUI
