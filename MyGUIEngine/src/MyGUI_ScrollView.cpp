/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ScrollView.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_ISubWidgetText.h"
#include "MyGUI_ScrollBar.h"

namespace MyGUI
{

	const int SCROLL_VIEW_MOUSE_WHEEL = 50; // колличество пикселей для колеса мыши
	const int SCROLL_VIEW_SCROLL_PAGE = 16; // колличество пикселей для кнопок скрола

	ScrollView::ScrollView() :
		mContentAlign(Align::Center)
	{
		mChangeContentByResize = false;
	}

	void ScrollView::initialiseOverride()
	{
		Base::initialiseOverride();

		// FIXME нам нужен фокус клавы
		setNeedKeyFocus(true);

		assignWidget(mScrollViewClient, "Client");
		if (getClientWidget() != nullptr)
		{
			getClientWidget()->eventMouseWheel += newDelegate(this, &ScrollView::notifyMouseWheel);
		}

		// create widget that will be real parent for child widgets
		Widget* realClient = _getClientWidget()->createWidget<Widget>("Default", IntCoord(), Align::Default);
		realClient->eventMouseWheel += newDelegate(this, &ScrollView::notifyMouseWheel);
		setWidgetClient(realClient);

		///@wskin_child{ScrollView, ScrollBar, VScroll} Вертикальная полоса прокрутки.
		assignWidget(mVScroll, "VScroll");
		if (mVScroll != nullptr)
		{
			mVScroll->eventScrollChangePosition += newDelegate(this, &ScrollView::notifyScrollChangePosition);
		}

		///@wskin_child{ScrollView, ScrollBar, HScroll} Горизонтальная полоса прокрутки.
		assignWidget(mHScroll, "HScroll");
		if (mHScroll != nullptr)
		{
			mHScroll->eventScrollChangePosition += newDelegate(this, &ScrollView::notifyScrollChangePosition);
		}

		updateView();
	}

	void ScrollView::shutdownOverride()
	{
		mVScroll = nullptr;
		mHScroll = nullptr;
		mScrollViewClient = nullptr;

		Base::shutdownOverride();
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

	void ScrollView::notifyScrollChangePosition(ScrollBar* _sender, size_t _position)
	{
		if (_sender == mVScroll)
		{
			IntPoint point = getClientWidget()->getPosition();
			point.top = -(int)_position;
			getClientWidget()->setPosition(point);
		}
		else if (_sender == mHScroll)
		{
			IntPoint point = getClientWidget()->getPosition();
			point.left = -(int)_position;
			getClientWidget()->setPosition(point);
		}
	}

	void ScrollView::notifyMouseWheel(Widget* _sender, int _rel)
	{
		if (mVRange != 0)
		{
			IntPoint point = getClientWidget()->getPosition();
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
				getClientWidget()->setPosition(point);
			}
		}
		else if (mHRange != 0)
		{
			IntPoint point = getClientWidget()->getPosition();
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
				getClientWidget()->setPosition(point);
			}
		}
	}

	IntSize ScrollView::getContentSize()
	{
		return getClientWidget()->getSize();
	}

	IntPoint ScrollView::getContentPosition()
	{
		return IntPoint() - getClientWidget()->getPosition();
	}

	void ScrollView::setContentPosition(const IntPoint& _point)
	{
		getClientWidget()->setPosition(IntPoint() - _point);
	}

	IntSize ScrollView::getViewSize()
	{
		return mScrollViewClient == nullptr ? getSize() : mScrollViewClient->getSize();
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
		getClientWidget()->setSize(_value);
		updateView();
	}

	IntSize ScrollView::getCanvasSize()
	{
		return getClientWidget()->getSize();
	}

	void ScrollView::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		/// @wproperty{ScrollView, VisibleVScroll, bool} Horizontal scroll bar visibility.
		if (_key == "VisibleVScroll")
			setVisibleVScroll(utility::parseValue<bool>(_value));

		/// @wproperty{ScrollView, VisibleHScroll, bool} Horizontal scroll bar visibility.
		else if (_key == "VisibleHScroll")
			setVisibleHScroll(utility::parseValue<bool>(_value));

		/// @wproperty{ScrollView, CanvasAlign, Align} Выравнивание содержимого.
		else if (_key == "CanvasAlign")
			setCanvasAlign(utility::parseValue<Align>(_value));

		/// @wproperty{ScrollView, CanvasSize, int int} Размер содержимого.
		else if (_key == "CanvasSize")
			setCanvasSize(utility::parseValue<IntSize>(_value));

		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}

		eventChangeProperty(this, _key, _value);
	}

	bool ScrollView::isVisibleVScroll() const
	{
		return mVisibleVScroll;
	}

	bool ScrollView::isVisibleHScroll() const
	{
		return mVisibleHScroll;
	}

	Align ScrollView::getCanvasAlign() const
	{
		return mContentAlign;
	}

	void ScrollView::setCanvasSize(int _width, int _height)
	{
		setCanvasSize(IntSize(_width, _height));
	}

	Align ScrollView::getContentAlign()
	{
		return mContentAlign;
	}

	void ScrollView::setViewOffset(const IntPoint& _value)
	{
		IntPoint value = _value;
		IntPoint currentOffset = getClientWidget()->getPosition();

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

		getClientWidget()->setPosition(value);
	}

	IntPoint ScrollView::getViewOffset() const
	{
		return getClientWidget()->getPosition();
	}

	IntCoord ScrollView::getViewCoord() const
	{
		return mScrollViewClient == nullptr ? getCoord() : mScrollViewClient->getCoord();
	}

	ScrollBar* ScrollView::getVScroll()
	{
		return mVScroll;
	}

} // namespace MyGUI
