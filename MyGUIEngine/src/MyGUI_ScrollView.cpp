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
		mContentAlign(Align::Center),
		mRealClient(nullptr)
	{
		mChangeContentByResize = false;
	}

	void ScrollView::initialiseOverride()
	{
		Base::initialiseOverride();

		// FIXME нам нужен фокус клавы
		setNeedKeyFocus(true);

		///@wskin_child{ScrollView, Widget, Client} Клиентская зона.
		assignWidget(mClient, "Client");
		MyGUI::Widget* realClientOwner = this;
		if (mClient != nullptr)
		{
			mClient->eventMouseWheel += newDelegate(this, &ScrollView::notifyMouseWheel);
			realClientOwner = mClient;
		}

		// создаем холcт, реальный владелец детей
		mRealClient = realClientOwner->createWidget<Widget>("Default", IntCoord(), Align::Default);
		mRealClient->eventMouseWheel += newDelegate(this, &ScrollView::notifyMouseWheel);
		setWidgetClient(mRealClient);

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
		mClient = nullptr;
		mRealClient = nullptr;

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

	void ScrollView::notifyMouseWheel(Widget* _sender, int _rel)
	{
		if (mRealClient == nullptr)
			return;

		if (mVRange != 0)
		{
			IntPoint point = mRealClient->getPosition();
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

	IntSize ScrollView::getContentSize()
	{
		return mRealClient == nullptr ? IntSize() : mRealClient->getSize();
	}

	IntPoint ScrollView::getContentPosition()
	{
		return mRealClient == nullptr ? IntPoint() : (IntPoint() - mRealClient->getPosition());
	}

	void ScrollView::setContentPosition(const IntPoint& _point)
	{
		if (mRealClient != nullptr)
			mRealClient->setPosition(IntPoint() - _point);
	}

	IntSize ScrollView::getViewSize()
	{
		return mClient == nullptr ? getSize() : mClient->getSize();
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
		if (mRealClient != nullptr)
			mRealClient->setSize(_value);
		updateView();
	}

	IntSize ScrollView::getCanvasSize()
	{
		return mRealClient == nullptr ? IntSize() : mRealClient->getSize();
	}

	void ScrollView::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		/// @wproperty{ScrollView, VisibleVScroll, bool} Видимость вертикальной полосы прокрутки.
		if (_key == "VisibleVScroll")
			setVisibleVScroll(utility::parseValue<bool>(_value));

		/// @wproperty{ScrollView, VisibleHScroll, bool} Видимость горизонтальной полосы прокрутки.
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

	void ScrollView::setPosition(int _left, int _top)
	{
		setPosition(IntPoint(_left, _top));
	}

	void ScrollView::setSize(int _width, int _height)
	{
		setSize(IntSize(_width, _height));
	}

	void ScrollView::setCoord(int _left, int _top, int _width, int _height)
	{
		setCoord(IntCoord(_left, _top, _width, _height));
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

	IntPoint ScrollView::getViewOffset() const
	{
		return mRealClient->getPosition();
	}

	IntCoord ScrollView::getViewCoord() const
	{
		return mClient == nullptr ? getCoord() : mClient->getCoord();
	}

	ScrollBar* ScrollView::getVScroll()
	{
		return mVScroll;
	}

} // namespace MyGUI
