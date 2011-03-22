/*!
	@file
	@author		Albert Semenov
	@date		03/2011
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
#include "ScrollViewPanel.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_ISubWidgetText.h"
#include "MyGUI_ScrollBar.h"

namespace MyGUI
{

	const int SCROLL_VIEW_MOUSE_WHEEL = 50; // ����������� �������� ��� ������ ����
	const int SCROLL_VIEW_SCROLL_PAGE = 16; // ����������� �������� ��� ������ ������

	ScrollViewPanel::ScrollViewPanel() :
		mContentAlign(Align::Center),
		mRealClient(nullptr)
	{
		mChangeContentByResize = false;
		mContentAlign = Align::Center;
	}

	void ScrollViewPanel::initialiseOverride()
	{
		Base::initialiseOverride();

		// FIXME ��� ����� ����� �����
		setNeedKeyFocus(true);

		assignWidget(mClient, "Client");
		if (mClient != nullptr)
		{
			mClient->eventMouseWheel += newDelegate(this, &ScrollViewPanel::notifyMouseWheel);

			// ������� ���c�, �������� �������� �����
			mRealClient = mClient->createWidget<Widget>("Default", IntCoord(), Align::Default);
			mRealClient->eventMouseWheel += newDelegate(this, &ScrollViewPanel::notifyMouseWheel);
			setWidgetClient(mRealClient);
		}

		assignWidget(mVScroll, "VScroll");
		if (mVScroll != nullptr)
		{
			mVScroll->eventScrollChangePosition += newDelegate(this, &ScrollViewPanel::notifyScrollChangePosition);
		}

		assignWidget(mHScroll, "HScroll");
		if (mHScroll != nullptr)
		{
			mHScroll->eventScrollChangePosition += newDelegate(this, &ScrollViewPanel::notifyScrollChangePosition);
		}

		updateView();
	}

	void ScrollViewPanel::shutdownOverride()
	{
		mVScroll = nullptr;
		mHScroll = nullptr;
		mClient = nullptr;
		mRealClient = nullptr;

		Base::shutdownOverride();
	}

	void ScrollViewPanel::setPosition(const IntPoint& _point)
	{
		Base::setPosition(_point);
	}

	void ScrollViewPanel::setSize(const IntSize& _size)
	{
		Base::setSize(_size);

		updateView();
		//overrideArrange();
	}

	void ScrollViewPanel::setCoord(const IntCoord& _coord)
	{
		Base::setCoord(_coord);

		updateView();
		//overrideArrange();
	}

	void ScrollViewPanel::notifyScrollChangePosition(ScrollBar* _sender, size_t _position)
	{
		if (mRealClient == nullptr)
			return;

		if (_sender == mVScroll)
		{
			IntPoint point = mRealClient->getPosition();
			point.top = -(int)_position;
			mRealClient->setPosition(point);
		}
		else if (_sender == mHScroll)
		{
			IntPoint point = mRealClient->getPosition();
			point.left = -(int)_position;
			mRealClient->setPosition(point);
		}
	}

	void ScrollViewPanel::notifyMouseWheel(Widget* _sender, int _rel)
	{
		if (mRealClient == nullptr)
			return;

		if (mVRange != 0)
		{
			IntPoint point = mRealClient->getPosition();
			int offset = -point.top;
			if (_rel < 0)
				offset += SCROLL_VIEW_MOUSE_WHEEL;
			else
				offset -= SCROLL_VIEW_MOUSE_WHEEL;

			if (offset < 0)
				offset = 0;
			else if (offset > (int)mVRange)
				offset = mVRange;

			if (offset != point.top)
			{
				point.top = -offset;
				if (mVScroll != nullptr)
				{
					mVScroll->setScrollPosition(offset);
				}
				mRealClient->setPosition(point);
			}
		}
		else if (mHRange != 0)
		{
			IntPoint point = mRealClient->getPosition();
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
				mRealClient->setPosition(point);
			}
		}
	}

	IntSize ScrollViewPanel::getContentSize()
	{
		return mRealClient == nullptr ? IntSize() : mRealClient->getSize();
	}

	IntPoint ScrollViewPanel::getContentPosition()
	{
		return mRealClient == nullptr ? IntPoint() : (IntPoint() - mRealClient->getPosition());
	}

	void ScrollViewPanel::setContentPosition(const IntPoint& _point)
	{
		if (mRealClient != nullptr)
			mRealClient->setPosition(IntPoint() - _point);
	}

	IntSize ScrollViewPanel::getViewSize()
	{
		return mClient == nullptr ? IntSize() : mClient->getSize();
	}

	size_t ScrollViewPanel::getVScrollPage()
	{
		return SCROLL_VIEW_SCROLL_PAGE;
	}

	size_t ScrollViewPanel::getHScrollPage()
	{
		return SCROLL_VIEW_SCROLL_PAGE;
	}

	void ScrollViewPanel::updateView()
	{
		updateScrollSize();
		updateScrollPosition();
	}

	void ScrollViewPanel::setVisibleVScroll(bool _value)
	{
		mVisibleVScroll = _value;
		updateView();
	}

	void ScrollViewPanel::setVisibleHScroll(bool _value)
	{
		mVisibleHScroll = _value;
		updateView();
	}

	void ScrollViewPanel::setCanvasAlign(Align _value)
	{
		mContentAlign = _value;
		updateView();
	}

	void ScrollViewPanel::setCanvasSize(const IntSize& _value)
	{
		if (mRealClient != nullptr)
			mRealClient->setSize(_value);
		updateView();
	}

	IntSize ScrollViewPanel::getCanvasSize()
	{
		return mRealClient == nullptr ? IntSize() : mRealClient->getSize();
	}

	void ScrollViewPanel::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		if (_key == "VisibleVScroll")
			setVisibleVScroll(utility::parseValue<bool>(_value));
		else if (_key == "VisibleHScroll")
			setVisibleHScroll(utility::parseValue<bool>(_value));
		else if (_key == "CanvasAlign")
			setCanvasAlign(utility::parseValue<Align>(_value));
		else if (_key == "CanvasSize")
			setCanvasSize(utility::parseValue<IntSize>(_value));
		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}
		eventChangeProperty(this, _key, _value);
	}

	void ScrollViewPanel::setPosition(int _left, int _top)
	{
		setPosition(IntPoint(_left, _top));
	}

	void ScrollViewPanel::setSize(int _width, int _height)
	{
		setSize(IntSize(_width, _height));
	}

	void ScrollViewPanel::setCoord(int _left, int _top, int _width, int _height)
	{
		setCoord(IntCoord(_left, _top, _width, _height));
	}

	bool ScrollViewPanel::isVisibleVScroll() const
	{
		return mVisibleVScroll;
	}

	bool ScrollViewPanel::isVisibleHScroll() const
	{
		return mVisibleHScroll;
	}

	Align ScrollViewPanel::getCanvasAlign() const
	{
		return mContentAlign;
	}

	void ScrollViewPanel::setCanvasSize(int _width, int _height)
	{
		setCanvasSize(IntSize(_width, _height));
	}

	Align ScrollViewPanel::getContentAlign()
	{
		return mContentAlign;
	}

	void ScrollViewPanel::setViewOffset(const IntPoint& _value)
	{
		IntPoint value = _value;
		IntPoint currentOffset = mRealClient->getPosition();

		if (mHRange != 0)
		{
			if (value.left > 0)
				value.left = 0;
			else if (value.left < -(int)mHRange)
				value.left = -(int)mHRange;
		}
		else
		{
			value.left = currentOffset.left;
		}

		if (mVRange != 0)
		{
			if (value.top > 0)
				value.top = 0;
			else if (value.top < -(int)mVRange)
				value.top = -(int)mVRange;
		}
		else
		{
			value.top = currentOffset.top;
		}

		if (mHScroll != nullptr)
			mHScroll->setScrollPosition(-value.left);

		if (mVScroll != nullptr)
			mVScroll->setScrollPosition(-value.top);

		mRealClient->setPosition(value);
	}

	IntPoint ScrollViewPanel::getViewOffset()
	{
		return mRealClient->getPosition();
	}

	IntCoord ScrollViewPanel::getViewCoord()
	{
		return mClient == nullptr ? IntCoord() : mClient->getCoord();
	}

	IntSize ScrollViewPanel::overrideMeasure(const IntSize& _sizeAvailable)
	{
		IntSize result;

		EnumeratorWidgetPtr child = getEnumerator();
		while (child.next())
		{
			Panel::updateMeasure(child.current(), IntSize((std::numeric_limits<int>::max)(), (std::numeric_limits<int>::max)()));
			result = Panel::getDesiredSize(child.current());

			break;
		}

		return result;
	}

	void ScrollViewPanel::overrideArrange()
	{
		EnumeratorWidgetPtr child = getEnumerator();
		while (child.next())
		{
			const IntSize& childSize = Panel::getDesiredSize(child.current());
			setCanvasSize(childSize);
			Panel::updateArrange(child.current(), IntCoord(0, 0, childSize.width, childSize.height));

			break;
		}
	}

} // namespace MyGUI
