/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ControllerEdgeHide.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

#ifdef M_PI
	#undef M_PI
#endif
	const float M_PI = 3.141593f;

	void ControllerEdgeHide::prepareItem(Widget* _widget)
	{
		recalculateTime(_widget);
		// вызываем пользовательский делегат для подготовки
		eventPreAction(_widget, this);
	}

	bool ControllerEdgeHide::addTime(Widget* _widget, float _time)
	{
		const IntSize& view_size = _widget->getParentSize();
		// do nothing if we have minimized window
		if (view_size.width <= 1 && view_size.height <= 1)
			return true;

		Widget* keyFocus = InputManager::getInstance().getKeyFocusWidget();
		Widget* mouseFocus = InputManager::getInstance().getMouseFocusWidget();

		while ((keyFocus != nullptr) && (_widget != keyFocus))
			keyFocus = keyFocus->getParent();
		while ((mouseFocus != nullptr) && (_widget != mouseFocus))
			mouseFocus = mouseFocus->getParent();

		// if our widget or its children have focus
		bool haveFocus = ((keyFocus != nullptr) || (mouseFocus != nullptr)) || (!_widget->getVisible());

		mElapsedTime += haveFocus ? -_time : _time;

		if (mElapsedTime >= mTime)
		{
			mElapsedTime = mTime;
		}
		if (mElapsedTime <= 0)
		{
			mElapsedTime = 0.0f;
			return true;
		}

		float k = std::sin(M_PI * mElapsedTime / mTime - M_PI / 2);
		if (k < 0)
			k = (-std::pow(-k, 0.7f) + 1) / 2;
		else
			k = (std::pow(k, 0.7f) + 1) / 2;

		MyGUI::IntCoord coord = _widget->getCoord();
		// if widget was moved
		if (coord != mLastCoord)
		{
			// if still moving - leave it alone
			if (haveFocus)
				return true;
			recalculateTime(_widget);
		}

		bool nearBorder = false;

		bool behindLeft = coord.left <= 0;
		bool behindRight = coord.right() >= view_size.width - 1;
		bool behindTop = coord.top <= 0;
		bool behindBottom = coord.bottom() >= view_size.height - 1;
		if (behindLeft && !behindRight)
		{
			coord.left = -int(float(coord.width - mRemainPixels - mShadowSize) * k);
			nearBorder = true;
		}
		if (behindTop && !behindBottom)
		{
			coord.top = -int(float(coord.height - mRemainPixels - mShadowSize) * k);
			nearBorder = true;
		}
		if (behindRight && !behindLeft)
		{
			coord.left = int(float(view_size.width - 1) - float(mRemainPixels) * k - float(coord.width) * (1.f - k));
			nearBorder = true;
		}
		if (behindBottom && !behindTop)
		{
			coord.top = int(float(view_size.height - 1) - float(mRemainPixels) * k - float(coord.height) * (1.f - k));
			nearBorder = true;
		}

		if (nearBorder)
		{
			_widget->setCoord(coord);
		}
		else
		{
			mElapsedTime = 0;
		}
		mLastCoord = coord;

		eventUpdateAction(_widget, this);

		return true;
	}

	void ControllerEdgeHide::setProperty(std::string_view _key, std::string_view _value)
	{
		if (_key == "Time")
			setTime(utility::parseValue<float>(_value));
		else if (_key == "RemainPixels")
			setRemainPixels(utility::parseValue<int>(_value));
		else if (_key == "ShadowSize")
			setShadowSize(utility::parseValue<int>(_value));
	}

	void ControllerEdgeHide::recalculateTime(Widget* _widget)
	{
		float k = 0;
		const MyGUI::IntCoord& coord = _widget->getCoord();
		const MyGUI::IntSize& view_size = _widget->getParentSize();

		bool behindLeft = coord.left <= 0;
		bool behindRight = coord.right() >= view_size.width - 1;
		bool behindTop = coord.top <= 0;
		bool behindBottom = coord.bottom() >= view_size.height - 1;
		// check if widget is near any border and not near opposite borders at same time
		if (behindLeft && !behindRight)
		{
			k = -(float)coord.left / (coord.width - mRemainPixels - mShadowSize);
		}
		else if (behindTop && !behindBottom)
		{
			k = -(float)coord.top / (coord.height - mRemainPixels - mShadowSize);
		}
		else if (behindRight && !behindLeft)
		{
			k = (float)(coord.right() - view_size.width + 1) / (coord.width - mRemainPixels);
		}
		else if (behindBottom && !behindTop)
		{
			k = (float)(coord.bottom() - view_size.height + 1) / (coord.height - mRemainPixels);
		}

		//mElapsedTime = (asin(k)/M_PI + 1./2) * mTime;
		// this is reversed formula from ControllerEdgeHide::addTime k calculation
		if (k > 0.5f)
			mElapsedTime = (std::asin(std::pow(2 * k - 1, 1 / 0.7f)) / M_PI + 1.f / 2) * mTime;
		else
			mElapsedTime = (std::asin(-std::pow(-2 * k + 1, 1 / 0.7f)) / M_PI + 1.f / 2) * mTime;
	}

	void ControllerEdgeHide::setTime(float _value)
	{
		mTime = _value;
	}

	void ControllerEdgeHide::setRemainPixels(int _value)
	{
		mRemainPixels = _value;
	}

	void ControllerEdgeHide::setShadowSize(int _value)
	{
		mShadowSize = _value;
	}

} // namespace MyGUI
