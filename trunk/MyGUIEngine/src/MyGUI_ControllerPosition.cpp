/*!
	@file
	@author		Albert Semenov
	@date		01/2008
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

	ControllerPosition::ControllerPosition()
	{
	}

	ControllerPosition::ControllerPosition(float _speed) :
		mSpeed(_speed), mAngle(0), mDelta1(0), mDelta2(0)
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

		IntCoord rect = _widget->getCoord();
		int x = rect.width;
		int y = rect.height;
		mRadius = sqrt(float(x*x + y*y));
		mAngle = asin(x/mRadius);

		// вызываем пользовательский делегат для подготовки
		eventPreAction(_widget);
	}

	void ControllerPosition::replaseItem(WidgetPtr _widget, ControllerItem * _item)
	{
		ControllerPosition * item = static_cast<ControllerPosition*>(_item);
	}

	bool ControllerPosition::addTime(WidgetPtr _widget, float _time)
	{
		IntCoord rect = _widget->getCoord();

		if (mRadius > 0)
		{
			mDelta1 += -mRadius*_time*mSpeed*sin(mAngle);
			mDelta2 += mRadius*_time*mSpeed*cos(mAngle);
			while ( mDelta1 > 1 ){
				rect += IntCoord(mDelta1, 0, 0, -abs(mDelta1));
				mDelta1 -= 1;
			}
			while ( mDelta1 < -1 ){
				rect += IntCoord(mDelta1, 0, 0, -abs(mDelta1));
				mDelta1 += 1;
			}
			while ( mDelta2 > 1 ){
				rect += IntCoord(0, mDelta2, -abs(mDelta2), 0);
				mDelta2 -= 1;
			}
			while ( mDelta2 < -1 ){
				rect += IntCoord(0, mDelta2, -abs(mDelta2), 0);
				mDelta2 += 1;
			}
			_widget->setPosition(rect);
			mRadius -= 10*_time * mSpeed;
			mAngle += 10*_time * mSpeed;
			return true;
		}

		// вызываем пользовательский елегат пост обработки
		eventPostAction(_widget);

		return false;
	}

} // namespace MyGUI
