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

		float k;
		MyGUI::IntCoord coord = _widget->getCoord();
		if ((coord.left <= 0) && !(coord.right() >= (int)MyGUI::Gui::getInstance().getViewWidth()))
		{
			k = - coord.left / (coord.width - mRemainPixels - mShadowSize);
		}
		if ((coord.top <= 0) && !(coord.bottom() >= (int)MyGUI::Gui::getInstance().getViewHeight()))
		{
			k = - coord.top / (coord.height - mRemainPixels - mShadowSize);
		}
		if ((coord.right() >= (int)MyGUI::Gui::getInstance().getViewWidth()) && !(coord.left <= 0))
		{
			k = 1 + (coord.left - MyGUI::Gui::getInstance().getViewWidth() - mRemainPixels) / coord.width;
		}
		if ((coord.bottom() >= (int)MyGUI::Gui::getInstance().getViewHeight()) && !(coord.top <= 0))
		{
			k = 1 + (coord.top - MyGUI::Gui::getInstance().getViewHeight() - mRemainPixels) / coord.height;
		}

		mElapsedTime = (asin(k) + 1./2) * mTime;

		// вызываем пользовательский делегат для подготовки
		eventPreAction(_widget);
	}

	bool ControllerEdgeHide::addTime(WidgetPtr _widget, float _time)
	{
		WidgetPtr keyFocus = InputManager::getInstance().getKeyFocusWidget();
		WidgetPtr mouseFocus = InputManager::getInstance().getMouseFocusWidget();

		while ((keyFocus != nullptr) && (_widget != keyFocus))
			keyFocus = keyFocus->getParent();
		while ((mouseFocus != nullptr) && (_widget != mouseFocus))
			mouseFocus = mouseFocus->getParent();

		// if our widget or its children have focus
		bool haveFocus = ((keyFocus != nullptr) || (mouseFocus != nullptr)) || (_widget->isVisible() == false);

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

		eventUpdateAction(_widget);

		return true;
	}

} // namespace MyGUI
