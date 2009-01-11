/*!
	@file
	@author		Evmenov Georgiy
	@date		03/2008
	@module
*/
#include "MyGUI_Precompiled.h"
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

	bool ControllerEdgeHide::addTime(WidgetPtr _widget, float _time)
	{
		WidgetPtr keyFocus = InputManager::getInstance().getKeyFocusWidget();
		WidgetPtr mouseFocus = InputManager::getInstance().getMouseFocusWidget();
		WidgetPtr keyFocusOwner = InputManager::getInstance().getKeyFocusWidget();
		WidgetPtr mouseFocusOwner = InputManager::getInstance().getMouseFocusWidget();

		while ((keyFocus != nullptr) && (_widget != keyFocus))
			keyFocus = keyFocus->getParent();
		while ((mouseFocus != nullptr) && (_widget != mouseFocus))
			mouseFocus = mouseFocus->getParent();
		while ((keyFocusOwner != nullptr) && (_widget != keyFocusOwner))
			keyFocusOwner = keyFocusOwner->getParent();
		while ((mouseFocusOwner != nullptr) && (_widget != mouseFocusOwner))
			mouseFocusOwner = mouseFocusOwner->getParent();

		// if our widget or his children have focus
		bool haveFocus = ((keyFocus != nullptr) || (mouseFocus != nullptr)) || ((keyFocusOwner != nullptr) || (mouseFocusOwner != nullptr)) || (_widget->isShow() == false);

		mElapsedTime += (1 - 2*haveFocus) * _time;

		if (mElapsedTime >= mTime)
		{
			mElapsedTime = mTime;
			return true;
		}
		if (mElapsedTime <= 0)
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

		if ((coord.left <= 0) && !(coord.right() >= (int)MyGUI::Gui::getInstance().getViewWidth()))
		{
			coord.left = - int( float(coord.width - mRemainPixels - mShadowSize) * k);
			nearBorder = true;
		}
		if ((coord.top <= 0) && !(coord.bottom() >= (int)MyGUI::Gui::getInstance().getViewHeight()))
		{
			coord.top = - int( float(coord.height - mRemainPixels - mShadowSize) * k);
			nearBorder = true;
		}
		if ((coord.right() >= (int)MyGUI::Gui::getInstance().getViewWidth()) && !(coord.left <= 0))
		{
			coord.left = int(MyGUI::Gui::getInstance().getViewWidth() - float(mRemainPixels) - float(coord.width) * (float(1) - k));
			nearBorder = true;
		}
		if ((coord.bottom() >= (int)MyGUI::Gui::getInstance().getViewHeight()) && !(coord.top <= 0))
		{
			coord.top = int(MyGUI::Gui::getInstance().getViewHeight() - float(mRemainPixels) - float(coord.height) * (float(1) - k));
			nearBorder = true;
		}

		if (!nearBorder) mElapsedTime = 0;

		_widget->setCoord(coord);

		return true;
	}

} // namespace MyGUI
