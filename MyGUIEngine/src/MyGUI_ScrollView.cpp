/*!
	@file
	@author		Albert Semenov
	@date		08/2008
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
#include "MyGUI_ScrollView.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_ISubWidgetText.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_HScroll.h"

namespace MyGUI
{

	const int SCROLL_VIEW_MOUSE_WHEEL = 50; // колличество пикселей для колеса мыши
	const int SCROLL_VIEW_SCROLL_PAGE = 16; // колличество пикселей для кнопок скрола

	ScrollView::ScrollView() :
		mScrollClient(nullptr),
		mContentAlign(Align::Center)
	{
		mChangeContentByResize = false;
		mContentAlign = Align::Center;
	}

	void ScrollView::initialiseWidgetSkin(ResourceSkin* _info)
	{
		Base::initialiseWidgetSkin(_info);

		// FIXME нам нужен фокус клавы
		//mNeedKeyFocus = true;
		setNeedKeyFocus(true);

		for (VectorWidgetPtr::iterator iter=mWidgetChildSkin.begin(); iter!=mWidgetChildSkin.end(); ++iter)
		{
			if (*(*iter)->_getInternalData<std::string>() == "Client")
			{
				MYGUI_DEBUG_ASSERT( ! mScrollClient, "widget already assigned");
				mScrollClient = (*iter);
				mScrollClient->eventMouseWheel += newDelegate(this, &ScrollView::notifyMouseWheel);
				mClient = mScrollClient;

				// создаем холcт, реальный владелец детей
				mWidgetClient = mScrollClient->createWidget<Widget>("Default", IntCoord(), Align::Default);
				mWidgetClient->eventMouseWheel += newDelegate(this, &ScrollView::notifyMouseWheel);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "VScroll")
			{
				MYGUI_DEBUG_ASSERT( ! mVScroll, "widget already assigned");
				mVScroll = (*iter)->castType<VScroll>();
				mVScroll->eventScrollChangePosition += newDelegate(this, &ScrollView::notifyScrollChangePosition);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "HScroll")
			{
				MYGUI_DEBUG_ASSERT( ! mHScroll, "widget already assigned");
				mHScroll = (*iter)->castType<HScroll>();
				mHScroll->eventScrollChangePosition += newDelegate(this, &ScrollView::notifyScrollChangePosition);
			}
		}

		//MYGUI_ASSERT(nullptr != mScrollClient, "Child Widget Client not found in skin (ScrollView must have Client)");

		updateView();
	}

	void ScrollView::shutdownWidgetSkin()
	{
		mWidgetClient = nullptr;
		mVScroll = nullptr;
		mHScroll = nullptr;
		mScrollClient = nullptr;

		Base::shutdownWidgetSkin();
	}

	void ScrollView::setPosition(const IntPoint& _point)
	{
		Base::setPosition(_point);
	}

	void ScrollView::setSize(const IntSize& _size)
	{
		Base::setSize(_size);

		updateView();
	}

	void ScrollView::setCoord(const IntCoord& _coord)
	{
		Base::setCoord(_coord);

		updateView();
	}

	void ScrollView::notifyScrollChangePosition(VScroll* _sender, size_t _position)
	{
		if (mWidgetClient == nullptr)
			return;

		if (_sender == mVScroll)
		{
			IntPoint point = mWidgetClient->getPosition();
			point.top = -(int)_position;
			mWidgetClient->setPosition(point);
		}
		else if (_sender == mHScroll)
		{
			IntPoint point = mWidgetClient->getPosition();
			point.left = -(int)_position;
			mWidgetClient->setPosition(point);
		}
	}

	void ScrollView::notifyMouseWheel(Widget* _sender, int _rel)
	{
		if (mWidgetClient == nullptr)
			return;

		if (mVRange != 0)
		{
			IntPoint point = mWidgetClient->getPosition();
			int offset = -point.top;
			if (_rel < 0) offset += SCROLL_VIEW_MOUSE_WHEEL;
			else  offset -= SCROLL_VIEW_MOUSE_WHEEL;

			if (offset < 0) offset = 0;
			else if (offset > (int)mVRange) offset = mVRange;

			if (offset != point.top)
			{
				point.top = -offset;
				if (mVScroll != nullptr)
				{
					mVScroll->setScrollPosition(offset);
				}
				mWidgetClient->setPosition(point);
			}
		}
		else if (mHRange != 0)
		{
			IntPoint point = mWidgetClient->getPosition();
			int offset = -point.left;
			if (_rel < 0) offset += SCROLL_VIEW_MOUSE_WHEEL;
			else  offset -= SCROLL_VIEW_MOUSE_WHEEL;

			if (offset < 0) offset = 0;
			else if (offset > (int)mHRange) offset = mHRange;

			if (offset != point.left)
			{
				point.left = -offset;
				if (mHScroll != nullptr)
				{
					mHScroll->setScrollPosition(offset);
				}
				mWidgetClient->setPosition(point);
			}
		}
	}

	Widget* ScrollView::baseCreateWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name)
	{
		if (mWidgetClient == nullptr)
			return Base::baseCreateWidget(_style, _type, _skin, _coord, _align, _layer, _name);
		return mWidgetClient->createWidgetT(_style, _type, _skin, _coord, _align, _layer, _name);
	}

	IntSize ScrollView::getContentSize()
	{
		return mWidgetClient == nullptr ? IntSize() : mWidgetClient->getSize();
	}

	IntPoint ScrollView::getContentPosition()
	{
		return mWidgetClient == nullptr ? IntPoint() : (IntPoint() - mWidgetClient->getPosition());
	}

	void ScrollView::setContentPosition(const IntPoint& _point)
	{
		if (mWidgetClient != nullptr)
			mWidgetClient->setPosition(IntPoint() - _point);
	}

	IntSize ScrollView::getViewSize() const
	{
		return mScrollClient == nullptr ? IntSize() : mScrollClient->getSize();
	}

	size_t ScrollView::getVScrollPage()
	{
		return SCROLL_VIEW_SCROLL_PAGE;
	}

	size_t ScrollView::getHScrollPage()
	{
		return SCROLL_VIEW_SCROLL_PAGE;
	}

	void ScrollView::updateView()
	{
		updateScrollSize();
		updateScrollPosition();
	}

	void ScrollView::setVisibleVScroll(bool _value)
	{
		mVisibleVScroll = _value;
		updateView();
	}

	void ScrollView::setVisibleHScroll(bool _value)
	{
		mVisibleHScroll = _value;
		updateView();
	}

	void ScrollView::setCanvasAlign(Align _value)
	{
		mContentAlign = _value;
		updateView();
	}

	void ScrollView::setCanvasSize(const IntSize& _value)
	{
		if (mWidgetClient != nullptr)
			mWidgetClient->setSize(_value);
		updateView();
	}

	const IntCoord& ScrollView::getClientCoord()
	{
		return mScrollClient == nullptr ? getCoord() : mScrollClient->getCoord();
	}

	IntSize ScrollView::getCanvasSize()
	{
		return mWidgetClient == nullptr ? IntSize() : mWidgetClient->getSize();
	}

	void ScrollView::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		if (_key == "ScrollView_VisibleVScroll") setVisibleVScroll(utility::parseValue<bool>(_value));
		else if (_key == "ScrollView_VisibleHScroll") setVisibleHScroll(utility::parseValue<bool>(_value));
		else if (_key == "ScrollView_CanvasAlign") setCanvasAlign(utility::parseValue<Align>(_value));
		else if (_key == "ScrollView_CanvasSize") setCanvasSize(utility::parseValue<IntSize>(_value));
		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}
		eventChangeProperty(this, _key, _value);
	}

} // namespace MyGUI
