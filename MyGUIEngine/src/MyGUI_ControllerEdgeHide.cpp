/*!
	@file
	@author		Evmenov Georgiy
	@date		03/2008
	@module
*/
/*
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
#include "MyGUI_ControllerEdgeHide.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	ControllerEdgeHide::ControllerEdgeHide() :
		mTime(0),
		mRemainPixels(0),
		mShadowSize(0)
	{
	}

	void ControllerEdgeHide::prepareItem(Widget* _widget)
	{
		if (mTime == 0.0) mTime = 0.000001;

		float k = 0;
		const MyGUI::IntCoord& coord = _widget->getCoord();
		const MyGUI::IntSize& view_size = Gui::getInstance().getViewSize();
		if ((coord.left <= 0) && !(coord.right() >= view_size.width))
		{
			k = - (float) coord.left / (coord.width - mRemainPixels - mShadowSize);
		}
		if ((coord.top <= 0) && !(coord.bottom() >= view_size.height))
		{
			k = - (float)coord.top / (coord.height - mRemainPixels - mShadowSize);
		}
		if ((coord.right() >= view_size.width) && !(coord.left <= 0))
		{
			k = 1.f + (coord.left - view_size.width - mRemainPixels) / coord.width;
		}
		if ((coord.bottom() >= view_size.height) && !(coord.top <= 0))
		{
			k = 1.f + (coord.top - view_size.height - mRemainPixels) / coord.height;
		}

		mElapsedTime = (asin(k) + 1./2) * mTime;

		// вызываем пользовательский делегат для подготовки
		eventPreAction(_widget);
	}

	bool ControllerEdgeHide::addTime(Widget* _widget, float _time)
	{
		Widget* keyFocus = InputManager::getInstance().getKeyFocusWidget();
		Widget* mouseFocus = InputManager::getInstance().getMouseFocusWidget();

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
		if (k<0) k = (-pow((-k), 0.7f) + 1)/2;
		else k = (pow((k), 0.7f) + 1)/2;

		MyGUI::IntCoord coord = _widget->getCoord();

		IntSize view_size;
		if (_widget->getCroppedParent() == nullptr)
			view_size = _widget->getLayer()->getSize();
		else
			view_size = ((Widget*)_widget->getCroppedParent())->getSize();

		bool nearBorder = false;

		if ((coord.left <= 0) && !(coord.right() >= view_size.width))
		{
			coord.left = - int( float(coord.width - mRemainPixels - mShadowSize) * k);
			nearBorder = true;
		}
		if ((coord.top <= 0) && !(coord.bottom() >= view_size.height))
		{
			coord.top = - int( float(coord.height - mRemainPixels - mShadowSize) * k);
			nearBorder = true;
		}
		if ((coord.right() >= view_size.width-1) && !(coord.left <= 0))
		{
			coord.left = int(float(view_size.width-1) - float(mRemainPixels)*k - float(coord.width) * (1.f - k));
			nearBorder = true;
		}
		if ((coord.bottom() >= view_size.height-1) && !(coord.top <= 0))
		{
			coord.top = int(float(view_size.height-1) - float(mRemainPixels)*k - float(coord.height) * (1.f - k));
			nearBorder = true;
		}

		if (nearBorder)
			_widget->setCoord(coord);
		else
			mElapsedTime = 0;

		eventUpdateAction(_widget);

		return true;
	}

	void ControllerEdgeHide::setProperty(const std::string& _key, const std::string& _value)
	{
		if (_key == "Time") setTime(utility::parseValue<float>(_value));
		else if (_key == "RemainPixels") setRemainPixels(utility::parseValue<int>(_value));
		else if (_key == "ShadowSize") setShadowSize(utility::parseValue<int>(_value));
	}

} // namespace MyGUI
