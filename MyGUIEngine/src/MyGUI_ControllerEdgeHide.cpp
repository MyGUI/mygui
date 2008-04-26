/*!
	@file
	@author		Evmenov Georgiy
	@date		03/2008
	@module
*/
#include "MyGUI_Prerequest.h"
#include "MyGUI_ControllerEdgeHide.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{
	ControllerEdgeHide::ControllerEdgeHide(float _time):
		mTime(_time)
	{
	}

	const std::string & ControllerEdgeHide::getType()
	{
		static std::string type("ControllerEdgeHide");
		return type;
	}

	void ControllerEdgeHide::prepareItem(WidgetPtr _widget)
	{
		MYGUI_DEBUG_ASSERT(mTime > 0, "Time must be > 0");

		// вызываем пользовательский делегат для подготовки
		eventPreAction(_widget);
	}

	void ControllerEdgeHide::replaseItem(WidgetPtr _widget, ControllerItem * _item)
	{
		ControllerEdgeHide * item = static_cast<ControllerEdgeHide*>(_item);
	}

	bool ControllerEdgeHide::addTime(WidgetPtr _widget, float _time)
	{
		WidgetPtr keyFocus = InputManager::getInstance().getKeyFocusWidget();
		WidgetPtr mouseFocus = InputManager::getInstance().getMouseFocusWidget();

		while ((keyFocus != null) && (_widget != keyFocus))
			keyFocus = keyFocus->getParent();
		while ((mouseFocus != null) && (_widget != mouseFocus))
			mouseFocus = mouseFocus->getParent();

		// if our widget or his children have focus
		bool haveFocus = ((keyFocus != null) || (mouseFocus != null)) || (_widget->isShow() == false);

		mElapsedTime += (1 - 2*haveFocus) * _time;

		if (mElapsedTime > mTime) mElapsedTime = mTime;
		if (mElapsedTime < 0) mElapsedTime = 0.0f;
		//mElapsedTime -= mTime * (int)(mElapsedTime/mTime);

		#ifndef M_PI
		const float M_PI = 3.141593;
		#endif
		float k = sin(M_PI * mElapsedTime/mTime - M_PI/2);
		if (k<0) k = (-pow((-k), (float)0.7) + 1)/2;
		else k = (pow((k), (float)0.7) + 1)/2;

		MyGUI::IntCoord coord = _widget->getCoord();
		bool nearBorder = false;
		if (coord.left <= 0)
		{
			coord.left = -coord.width * k;
			nearBorder = true;
		}
		if (coord.top <= 0)
		{
			coord.top = -coord.height * k;
			nearBorder = true;
		}
		if (coord.right() >= (int)MyGUI::Gui::getInstance().getViewWidth())
		{
			coord.left = (int)MyGUI::Gui::getInstance().getViewWidth() - coord.width * (1 - k);
			nearBorder = true;
		}
		if (coord.bottom() >= (int)MyGUI::Gui::getInstance().getViewHeight())
		{
			coord.top = (int)MyGUI::Gui::getInstance().getViewHeight() - coord.height * (1 - k);
			nearBorder = true;
		}

		if (!nearBorder) mElapsedTime = 0;

		_widget->setPosition(coord);

		return true;
	}

} // namespace MyGUI
