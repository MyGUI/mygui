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
	ControllerEdgeHide::ControllerEdgeHide(float _time, int _remainPixels, int _shadowSize ):
		mTime(_time), mRemainPixels(_remainPixels), mShadowSize(_shadowSize)
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
		WidgetPtr keyFocusOwner = InputManager::getInstance().getKeyFocusWidget();
		WidgetPtr mouseFocusOwner = InputManager::getInstance().getMouseFocusWidget();

		while ((keyFocus != null) && (_widget != keyFocus))
			keyFocus = keyFocus->getParent();
		while ((mouseFocus != null) && (_widget != mouseFocus))
			mouseFocus = mouseFocus->getParent();
		while ((keyFocusOwner != null) && (_widget != keyFocusOwner))
			keyFocusOwner = keyFocusOwner->_getOwner();
		while ((mouseFocusOwner != null) && (_widget != mouseFocusOwner))
			mouseFocusOwner = mouseFocusOwner->_getOwner();

		// if our widget or his children have focus
		bool haveFocus = ((keyFocus != null) || (mouseFocus != null)) || ((keyFocusOwner != null) || (mouseFocusOwner != null)) || (_widget->isShow() == false);

		mElapsedTime += (1 - 2*haveFocus) * _time;

		if (mElapsedTime > mTime)
		{
			mElapsedTime = mTime;
			return true;
		}
		if (mElapsedTime < 0)
		{
			mElapsedTime = 0.0f;
			return true;
		}

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
			coord.left = -(coord.width - mRemainPixels - mShadowSize) * k;
			nearBorder = true;
		}
		if (coord.top <= 0)
		{
			coord.top = -(coord.height - mRemainPixels - mShadowSize) * k;
			nearBorder = true;
		}
		if (coord.right() >= (int)MyGUI::Gui::getInstance().getViewWidth())
		{
			coord.left = (int)MyGUI::Gui::getInstance().getViewWidth() - mRemainPixels - (coord.width) * (1 - k);
			nearBorder = true;
		}
		if (coord.bottom() >= (int)MyGUI::Gui::getInstance().getViewHeight())
		{
			coord.top = (int)MyGUI::Gui::getInstance().getViewHeight() - mRemainPixels - (coord.height) * (1 - k);
			nearBorder = true;
		}

		if (!nearBorder) mElapsedTime = 0;

		_widget->setPosition(coord);

		return true;
	}

} // namespace MyGUI
