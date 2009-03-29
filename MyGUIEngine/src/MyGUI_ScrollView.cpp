/*!
	@file
	@author		Albert Semenov
	@date		08/2008
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
#include "MyGUI_ScrollView.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_ISubWidgetText.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_HScroll.h"

namespace MyGUI
{

	const int SCROLL_VIEW_MOUSE_WHEEL = 50; // колличество пикселей для колеса мыши
	const int SCROLL_VIEW_SCROLL_PAGE = 16; // колличество пикселей для кнопок скрола

	ScrollView::ScrollView(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Base(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name),
		mIsFocus(false),
		mIsPressed(false),
		mScrollClient(nullptr),
		mContentAlign(Align::Center)
	{
		mChangeContentByResize = false;
		mContentAlign = Align::Center;

		initialiseWidgetSkin(_info);
	}

	ScrollView::~ScrollView()
	{
		shutdownWidgetSkin();
	}

	void ScrollView::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Base::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void ScrollView::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
		// нам нужен фокус клавы
		mNeedKeyFocus = true;

		for (VectorWidgetPtr::iterator iter=mWidgetChildSkin.begin(); iter!=mWidgetChildSkin.end(); ++iter)
		{
			if (*(*iter)->_getInternalData<std::string>() == "Client")
			{
				MYGUI_DEBUG_ASSERT( ! mScrollClient, "widget already assigned");
				mScrollClient = (*iter);
				mScrollClient->eventMouseSetFocus = newDelegate(this, &ScrollView::notifyMouseSetFocus);
				mScrollClient->eventMouseLostFocus = newDelegate(this, &ScrollView::notifyMouseLostFocus);
				mScrollClient->eventMouseWheel = newDelegate(this, &ScrollView::notifyMouseWheel);
				mClient = mScrollClient;

				// создаем холcт, реальный владелец детей
				mWidgetClient = mScrollClient->createWidget<Widget>("Default", IntCoord(), Align::Default);
				mWidgetClient->eventMouseWheel = newDelegate(this, &ScrollView::notifyMouseWheel);
				mWidgetClient->eventMouseSetFocus = newDelegate(this, &ScrollView::notifyMouseSetFocus);
				mWidgetClient->eventMouseLostFocus = newDelegate(this, &ScrollView::notifyMouseLostFocus);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "VScroll")
			{
				MYGUI_DEBUG_ASSERT( ! mVScroll, "widget already assigned");
				mVScroll = (*iter)->castType<VScroll>();
				mVScroll->eventScrollChangePosition = newDelegate(this, &ScrollView::notifyScrollChangePosition);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "HScroll")
			{
				MYGUI_DEBUG_ASSERT( ! mHScroll, "widget already assigned");
				mHScroll = (*iter)->castType<HScroll>();
				mHScroll->eventScrollChangePosition = newDelegate(this, &ScrollView::notifyScrollChangePosition);
			}
		}

		MYGUI_ASSERT(nullptr != mScrollClient, "Child Widget Client not found in skin (ScrollView must have Client)");

		updateView();
	}

	void ScrollView::shutdownWidgetSkin()
	{
		mWidgetClient = nullptr;
		mVScroll = nullptr;
		mHScroll = nullptr;
		mScrollClient = nullptr;
	}

	void ScrollView::notifyMouseSetFocus(WidgetPtr _sender, WidgetPtr _old)
	{
		if ( (_old == mScrollClient) || (mIsFocus) ) return;
		mIsFocus = true;
		updateScrollViewState();
	}

	void ScrollView::notifyMouseLostFocus(WidgetPtr _sender, WidgetPtr _new)
	{
		if ( (_new == mScrollClient) || (false == mIsFocus) ) return;
		mIsFocus = false;
		updateScrollViewState();
	}

	void ScrollView::onKeySetFocus(WidgetPtr _old)
	{
		if (false == mIsPressed)
		{
			mIsPressed = true;
			updateScrollViewState();
		}

		Base::onKeySetFocus(_old);
	}

	void ScrollView::onKeyLostFocus(WidgetPtr _new)
	{
		if (mIsPressed)
		{
			mIsPressed = false;
			updateScrollViewState();
		}

		Base::onKeyLostFocus(_new);
	}

	void ScrollView::updateScrollViewState()
	{
		if (!mEnabled) setState("disabled");
		else if (mIsPressed)
		{
			if (mIsFocus) setState("pushed");
			else setState("normal_checked");
		}
		else if (mIsFocus) setState("highlighted");
		else setState("normal");
	}

	void ScrollView::setPosition(const IntPoint & _point)
	{
		Base::setPosition(_point);
	}

	void ScrollView::setSize(const IntSize& _size)
	{
		Base::setSize(_size);

		updateView();
	}

	void ScrollView::setCoord(const IntCoord & _coord)
	{
		Base::setCoord(_coord);

		updateView();
	}

	void ScrollView::notifyScrollChangePosition(VScrollPtr _sender, size_t _position)
	{
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

	void ScrollView::notifyMouseWheel(WidgetPtr _sender, int _rel)
	{
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

	WidgetPtr ScrollView::baseCreateWidget(WidgetStyle _style, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name)
	{
		return mWidgetClient->createWidgetT(_style, _type, _skin, _coord, _align, _layer, _name);
	}

	IntSize ScrollView::getContentSize()
	{
		return mWidgetClient->getSize();
	}

	IntPoint ScrollView::getContentPosition()
	{
		return IntPoint() - mWidgetClient->getPosition();
	}

	void ScrollView::setContentPosition(const IntPoint& _point)
	{
		mWidgetClient->setPosition(IntPoint() - _point);
	}

	IntSize ScrollView::getViewSize()
	{
		return mScrollClient->getSize();
	}

	size_t ScrollView::getScrollPage()
	{
		return SCROLL_VIEW_SCROLL_PAGE;
	}

	void ScrollView::updateView()
	{
		updateScrollSize();
		updateScrollPosition();
	}

} // namespace MyGUI
