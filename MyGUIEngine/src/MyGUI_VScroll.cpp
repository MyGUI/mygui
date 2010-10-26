/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
#include "MyGUI_VScroll.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_Button.h"
#include "MyGUI_ResourceSkin.h"

namespace MyGUI
{

	// FIXME - move to widget property
	const int SCROLL_MOUSE_WHEEL = 50; // колличество пикселей для колеса мыши

	VScroll::VScroll() :
		mWidgetStart(nullptr),
		mWidgetEnd(nullptr),
		mWidgetTrack(nullptr),
		mWidgetFirstPart(nullptr),
		mWidgetSecondPart(nullptr),
		mSkinRangeStart(0),
		mSkinRangeEnd(0),
		mScrollRange(0),
		mScrollPosition(0),
		mScrollPage(0),
		mScrollViewPage(0),
		mMinTrackSize(0),
		mMoveToClick(false)
	{
	}

	void VScroll::initialiseOverride()
	{
		Base::initialiseOverride();

		// при нуле, будет игнорировать кнопки
		mScrollPage = 1;
		mScrollViewPage = 1;
		mMinTrackSize = 0;
		mSkinRangeStart = 0;
		mSkinRangeEnd = 0;

		assignWidget(mWidgetStart, "Start");
		if (mWidgetStart != nullptr)
		{
			mWidgetStart->eventMouseButtonPressed += newDelegate(this, &VScroll::notifyMousePressed);
			mWidgetStart->eventMouseWheel += newDelegate(this, &VScroll::notifyMouseWheel);
		}

		assignWidget(mWidgetEnd, "End");
		if (mWidgetEnd != nullptr)
		{
			mWidgetEnd->eventMouseButtonPressed += newDelegate(this, &VScroll::notifyMousePressed);
			mWidgetEnd->eventMouseWheel += newDelegate(this, &VScroll::notifyMouseWheel);
		}

		assignWidget(mWidgetTrack, "Track");
		if (mWidgetTrack)
		{
			mWidgetTrack->eventMouseDrag += newDelegate(this, &VScroll::notifyMouseDrag);
			mWidgetTrack->eventMouseButtonPressed += newDelegate(this, &VScroll::notifyMousePressed);
			mWidgetTrack->eventMouseButtonReleased += newDelegate(this, &VScroll::notifyMouseReleased);
			mWidgetTrack->eventMouseWheel += newDelegate(this, &VScroll::notifyMouseWheel);
			mWidgetTrack->setVisible(false);
		}

		assignWidget(mWidgetFirstPart, "FirstPart");
		if (mWidgetFirstPart != nullptr)
		{
			mWidgetFirstPart->eventMouseButtonPressed += newDelegate(this, &VScroll::notifyMousePressed);
			mWidgetFirstPart->eventMouseWheel += newDelegate(this, &VScroll::notifyMouseWheel);
		}

		assignWidget(mWidgetSecondPart, "SecondPart");
		if (mWidgetSecondPart != nullptr)
		{
			mWidgetSecondPart->eventMouseButtonPressed += newDelegate(this, &VScroll::notifyMousePressed);
			mWidgetSecondPart->eventMouseWheel += newDelegate(this, &VScroll::notifyMouseWheel);
		}

		if (isUserString("MinTrackSize"))
			mMinTrackSize = utility::parseValue<int>(getUserString("MinTrackSize"));
		if (isUserString("TrackRangeMargins"))
			utility::parseComplex<size_t>(getUserString("TrackRangeMargins"), mSkinRangeStart, mSkinRangeEnd);

		if (mWidgetTrack != nullptr)
		{
			if (mWidgetTrack->isUserString("MinTrackSize"))
				mMinTrackSize = utility::parseValue<int>(mWidgetTrack->getUserString("MinTrackSize"));
		}
	}

	void VScroll::shutdownOverride()
	{
		mWidgetStart = nullptr;
		mWidgetEnd = nullptr;
		mWidgetTrack = nullptr;
		mWidgetFirstPart = nullptr;
		mWidgetSecondPart = nullptr;

		Base::shutdownOverride();
	}

	void VScroll::updateTrack()
	{
		if (mWidgetTrack == nullptr)
			return;

		_forcePeek(mWidgetTrack);
		// размер диапазана в пикселях
		int pos = getLineSize();

		// скрываем если диапазан маленький или места мало
		if ((mScrollRange < 2) || (pos <= mWidgetTrack->getHeight()))
		{
			mWidgetTrack->setVisible(false);
			if (nullptr != mWidgetFirstPart)
				mWidgetFirstPart->setSize(mWidgetFirstPart->getWidth(), pos / 2);
			if (nullptr != mWidgetSecondPart)
				mWidgetSecondPart->setCoord(mWidgetSecondPart->getLeft(), pos / 2 + (int)mSkinRangeStart, mWidgetSecondPart->getWidth(), pos - pos / 2);
			return;
		}
		// если скрыт то покажем
		if (!mWidgetTrack->getVisible())
		{
			mWidgetTrack->setVisible(true);
		}

		// и обновляем позицию
		pos = (int)(((size_t)(pos - getTrackSize()) * mScrollPosition) / (mScrollRange - 1) + mSkinRangeStart);

		mWidgetTrack->setPosition(mWidgetTrack->getLeft(), pos);
		if (nullptr != mWidgetFirstPart)
		{
			int height = pos - mWidgetFirstPart->getTop();
			mWidgetFirstPart->setSize(mWidgetFirstPart->getWidth(), height);
		}
		if (nullptr != mWidgetSecondPart)
		{
			int top = pos + mWidgetTrack->getHeight();
			int height = mWidgetSecondPart->getHeight() + mWidgetSecondPart->getTop() - top;
			mWidgetSecondPart->setCoord(mWidgetSecondPart->getLeft(), top, mWidgetSecondPart->getWidth(), height);
		}
	}

	void VScroll::TrackMove(int _left, int _top)
	{
		if (mWidgetTrack == nullptr)
			return;

		const IntPoint& point = InputManager::getInstance().getLastLeftPressed();

		// расчитываем позицию виджета
		int start = mPreActionOffset.top + (_top - point.top);
		if (start < (int)mSkinRangeStart)
			start = (int)mSkinRangeStart;
		else if (start > (getTrackPlaceLength() - (int)mSkinRangeEnd - mWidgetTrack->getHeight()))
			start = (getTrackPlaceLength() - (int)mSkinRangeEnd - mWidgetTrack->getHeight());
		if (mWidgetTrack->getTop() != start)
			mWidgetTrack->setPosition(mWidgetTrack->getLeft(), start);

		// расчитываем положение соответствующее позиции
		// плюс пол позиции
		int pos = start - (int)mSkinRangeStart + (getLineSize() - getTrackSize()) / (((int)mScrollRange - 1) * 2);
		// высчитываем ближайшее значение и обновляем
		pos = pos * (int)(mScrollRange - 1) / (getLineSize() - getTrackSize());

		// проверяем на выходы и изменения
		if (pos < 0)
			pos = 0;
		else if (pos >= (int)mScrollRange)
			pos = (int)mScrollRange - 1;
		if (pos == (int)mScrollPosition)
			return;

		mScrollPosition = pos;

		updateTrack();

		// отсылаем событие
		eventScrollChangePosition(this, (int)mScrollPosition);
	}

	void VScroll::notifyMousePressed(Widget* _sender, int _left, int _top, MouseButton _id)
	{
		// диспечерезируем нажатие своих детей как свое
		eventMouseButtonPressed(this, _left, _top, _id);

		if (MouseButton::Left != _id)
			return;

		if (mMoveToClick &&
			_sender != mWidgetTrack &&
			_sender != mWidgetStart &&
			_sender != mWidgetEnd)
		{
			if (mWidgetTrack != nullptr)
			{
				mPreActionOffset = InputManager::getInstance().getLastLeftPressed();
				const IntPoint& point = InputManager::getInstance().getMousePositionByLayer() - mWidgetTrack->getParent()->getAbsolutePosition();

				mPreActionOffset.left -= getTrackSize() / 2;
				mPreActionOffset.top -= getTrackSize() / 2;

				TrackMove(point.left, point.top);
			}
		}
		else if (_sender == mWidgetStart)
		{
			// минимальное значение
			if (mScrollPosition == 0)
				return;

			// расчитываем следующее положение
			if (mScrollPosition > mScrollPage)
				mScrollPosition -= mScrollPage;
			else
				mScrollPosition = 0;

			// оповещаем
			eventScrollChangePosition(this, (int)mScrollPosition);
			updateTrack();
		}
		else if (_sender == mWidgetEnd)
		{
			// максимальное значение
			if ((mScrollRange < 2) || (mScrollPosition >= (mScrollRange - 1)))
				return;

			// расчитываем следующее положение
			if ((mScrollPosition + mScrollPage) < (mScrollRange - 1))
				mScrollPosition += mScrollPage;
			else
				mScrollPosition = mScrollRange - 1;

			// оповещаем
			eventScrollChangePosition(this, (int)mScrollPosition);
			updateTrack();
		}
		else if (_sender == mWidgetFirstPart)
		{
			// минимальное значение
			if (mScrollPosition == 0)
				return;

			// расчитываем следующее положение
			if (mScrollPosition > mScrollViewPage)
				mScrollPosition -= mScrollViewPage;
			else
				mScrollPosition = 0;

			// оповещаем
			eventScrollChangePosition(this, (int)mScrollPosition);
			updateTrack();
		}
		else if (_sender == mWidgetSecondPart)
		{
			// максимальное значение
			if ((mScrollRange < 2) || (mScrollPosition >= (mScrollRange - 1)))
				return;

			// расчитываем следующее положение
			if ((mScrollPosition + mScrollViewPage) < (mScrollRange - 1))
				mScrollPosition += mScrollViewPage;
			else
				mScrollPosition = mScrollRange - 1;

			// оповещаем
			eventScrollChangePosition(this, (int)mScrollPosition);
			updateTrack();
		}
		else if (_sender == mWidgetTrack)
		{
			mPreActionOffset.left = _sender->getLeft();
			mPreActionOffset.top = _sender->getTop();
		}
	}

	void VScroll::notifyMouseReleased(Widget* _sender, int _left, int _top, MouseButton _id)
	{
		updateTrack();
	}

	void VScroll::notifyMouseDrag(Widget* _sender, int _left, int _top, MouseButton _id)
	{
		if (_id == MouseButton::Left)
			TrackMove(_left, _top);
	}

	void VScroll::setScrollRange(size_t _range)
	{
		if (_range == mScrollRange)
			return;

		mScrollRange = _range;
		mScrollPosition = (mScrollPosition < mScrollRange) ? mScrollPosition : 0;
		updateTrack();
	}

	void VScroll::setScrollPosition(size_t _position)
	{
		if (_position == mScrollPosition)
			return;

		if (_position >= mScrollRange)
			_position = 0;

		mScrollPosition = _position;
		updateTrack();
	}

	void VScroll::setPosition(const IntPoint& _point)
	{
		Base::setPosition(_point);
	}

	void VScroll::setSize(const IntSize& _size)
	{
		Base::setSize(_size);
		// обновляем трек
		updateTrack();
	}

	void VScroll::setCoord(const IntCoord& _coord)
	{
		Base::setCoord(_coord);
		// обновляем трек
		updateTrack();
	}

	void VScroll::setTrackSize(int _size)
	{
		if (mWidgetTrack != nullptr)
			mWidgetTrack->setSize(mWidgetTrack->getWidth(), ((int)_size < (int)mMinTrackSize) ? (int)mMinTrackSize : (int)_size);
		updateTrack();
	}

	int VScroll::getTrackSize()
	{
		return mWidgetTrack == nullptr ? 1 : mWidgetTrack->getHeight();
	}

	int VScroll::getLineSize()
	{
		return getTrackPlaceLength() - (int)(mSkinRangeStart + mSkinRangeEnd);
	}

	void VScroll::onMouseWheel(int _rel)
	{
		notifyMouseWheel(nullptr, _rel);

		Base::onMouseWheel(_rel);
	}

	void VScroll::notifyMouseWheel(Widget* _sender, int _rel)
	{
		if (mScrollRange < 2)
			return;

		int offset = mScrollPosition;
		if (_rel < 0)
			offset += SCROLL_MOUSE_WHEEL;
		else
			offset -= SCROLL_MOUSE_WHEEL;

		if (offset < 0)
			offset = 0;
		else if (offset > (int)(mScrollRange - 1))
			offset = mScrollRange - 1;

		if ((size_t)offset != mScrollPosition)
		{
			mScrollPosition = offset;
			// оповещаем
			eventScrollChangePosition(this, (int)mScrollPosition);
			updateTrack();
		}
	}

	void VScroll::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		if (_key == "Range")
			setScrollRange(utility::parseValue<size_t>(_value));
		else if (_key == "RangePosition")
			setScrollPosition(utility::parseValue<size_t>(_value));
		else if (_key == "Page")
			setScrollPage(utility::parseValue<size_t>(_value));
		else if (_key == "ViewPage")
			setScrollViewPage(utility::parseValue<size_t>(_value));
		else if (_key == "MoveToClick")
			setMoveToClick(utility::parseValue<bool>(_value));
		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}
		eventChangeProperty(this, _key, _value);
	}

	size_t VScroll::getScrollRange() const
	{
		return mScrollRange;
	}

	size_t VScroll::getScrollPosition() const
	{
		return mScrollPosition;
	}

	void VScroll::setScrollPage(size_t _value)
	{
		mScrollPage = _value;
	}

	size_t VScroll::getScrollPage() const
	{
		return mScrollPage;
	}

	void VScroll::setScrollViewPage(size_t _value)
	{
		mScrollViewPage = _value;
	}

	size_t VScroll::getScrollViewPage() const
	{
		return mScrollViewPage;
	}

	void VScroll::setMinTrackSize(int _value)
	{
		mMinTrackSize = _value;
	}

	int VScroll::getMinTrackSize() const
	{
		return mMinTrackSize;
	}

	void VScroll::setMoveToClick(bool _value)
	{
		mMoveToClick = _value;
	}

	bool VScroll::getMoveToClick() const
	{
		return mMoveToClick;
	}

	void VScroll::setPosition(int _left, int _top)
	{
		setPosition(IntPoint(_left, _top));
	}

	void VScroll::setSize(int _width, int _height)
	{
		setSize(IntSize(_width, _height));
	}

	void VScroll::setCoord(int _left, int _top, int _width, int _height)
	{
		setCoord(IntCoord(_left, _top, _width, _height));
	}

	int VScroll::getTrackPlaceLength()
	{
		if (mWidgetTrack != nullptr)
			return mWidgetTrack->getParent()->getHeight();
		return 0;
	}

} // namespace MyGUI
