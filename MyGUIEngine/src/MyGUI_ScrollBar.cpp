/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ScrollBar.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_Button.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_ControllerRepeatClick.h"
#include "MyGUI_ControllerManager.h"

namespace MyGUI
{

	void ScrollBar::initialiseOverride()
	{
		Base::initialiseOverride();

		// at zero, buttons will be ignored
		mScrollPage = 1;
		mScrollViewPage = 1;
		mScrollWheelPage = 1;
		mMinTrackSize = 0;
		mSkinRangeStart = 0;
		mSkinRangeEnd = 0;

		mRepeatTriggerTime = 0.5f;
		mRepeatStepTime = 0.1f;

		///@wskin_child{ScrollBar, Button, Start} Range start button.
		assignWidget(mWidgetStart, "Start");

		///@wskin_child{ScrollBar, Button, End} Range end button.
		assignWidget(mWidgetEnd, "End");

		///@wskin_child{ScrollBar, Button, Track} Track button.
		assignWidget(mWidgetTrack, "Track");
		if (mWidgetTrack)
		{
			mWidgetTrack->eventMouseDrag += newDelegate(this, &ScrollBar::notifyMouseDrag);
			mWidgetTrack->setVisible(false);
		}

		///@wskin_child{ScrollBar, Widget, FirstPart} First half scroll widget from start to track, on press acts as page scroll.
		assignWidget(mWidgetFirstPart, "FirstPart");

		///@wskin_child{ScrollBar, Widget, SecondPart} Second half scroll widget from track to end, on press acts as page scroll.
		assignWidget(mWidgetSecondPart, "SecondPart");

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

	void ScrollBar::shutdownOverride()
	{
		mWidgetStart = nullptr;
		mWidgetEnd = nullptr;
		mWidgetTrack = nullptr;
		mWidgetFirstPart = nullptr;
		mWidgetSecondPart = nullptr;

		Base::shutdownOverride();
	}

	void ScrollBar::updateTrack()
	{
		if (mWidgetTrack == nullptr)
			return;

		_forcePick(mWidgetTrack);
		int pos = getLineSize();

		if (mVerticalAlignment)
		{
			if ((mScrollRange < 2) || (pos <= mWidgetTrack->getHeight()))
			{
				mWidgetTrack->setVisible(false);
				if (nullptr != mWidgetFirstPart)
					mWidgetFirstPart->setSize(mWidgetFirstPart->getWidth(), pos / 2);
				if (nullptr != mWidgetSecondPart)
					mWidgetSecondPart->setCoord(
						mWidgetSecondPart->getLeft(),
						pos / 2 + (int)mSkinRangeStart,
						mWidgetSecondPart->getWidth(),
						pos - pos / 2);
				return;
			}
			if (!mWidgetTrack->getVisible())
				mWidgetTrack->setVisible(true);

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
				int height = getTrackPlaceLength() - top;
				mWidgetSecondPart->setCoord(mWidgetSecondPart->getLeft(), top, mWidgetSecondPart->getWidth(), height);
			}
		}
		else
		{
			if ((mScrollRange < 2) || (pos <= mWidgetTrack->getWidth()))
			{
				mWidgetTrack->setVisible(false);
				if (nullptr != mWidgetFirstPart)
					mWidgetFirstPart->setSize(pos / 2, mWidgetFirstPart->getHeight());
				if (nullptr != mWidgetSecondPart)
					mWidgetSecondPart->setCoord(
						pos / 2 + (int)mSkinRangeStart,
						mWidgetSecondPart->getTop(),
						pos - pos / 2,
						mWidgetSecondPart->getHeight());
				return;
			}
			if (!mWidgetTrack->getVisible())
				mWidgetTrack->setVisible(true);

			pos = (int)(((size_t)(pos - getTrackSize()) * mScrollPosition) / (mScrollRange - 1) + mSkinRangeStart);

			mWidgetTrack->setPosition(pos, mWidgetTrack->getTop());
			if (nullptr != mWidgetFirstPart)
			{
				int height = pos - mWidgetFirstPart->getLeft();
				mWidgetFirstPart->setSize(height, mWidgetFirstPart->getHeight());
			}
			if (nullptr != mWidgetSecondPart)
			{
				int left = pos + mWidgetTrack->getWidth();
				int width = getTrackPlaceLength() - left;
				mWidgetSecondPart->setCoord(left, mWidgetSecondPart->getTop(), width, mWidgetSecondPart->getHeight());
			}
		}
	}

	void ScrollBar::TrackMove(int _left, int _top)
	{
		if (mWidgetTrack == nullptr)
			return;

		const IntPoint point =
			InputManager::getInstance().getLastPressedPositionForWidget(MouseButton::Left, mWidgetTrack);

		if (mVerticalAlignment)
		{
			int start = mPreActionOffset.top + (_top - point.top);
			if (start < (int)mSkinRangeStart)
				start = (int)mSkinRangeStart;
			else if (start > (getTrackPlaceLength() - (int)mSkinRangeEnd - mWidgetTrack->getHeight()))
				start = (getTrackPlaceLength() - (int)mSkinRangeEnd - mWidgetTrack->getHeight());
			if (mWidgetTrack->getTop() != start)
				mWidgetTrack->setPosition(mWidgetTrack->getLeft(), start);

			int pos = 0;
			if (mScrollRange >= 2)
			{
				pos = start - (int)mSkinRangeStart + (getLineSize() - getTrackSize()) / (((int)mScrollRange - 1) * 2);
				pos = pos * (int)(mScrollRange - 1) / (getLineSize() - getTrackSize());
			}

			if (pos < 0)
				pos = 0;
			else if (pos >= (int)mScrollRange)
				pos = (int)mScrollRange - 1;
			if (pos == (int)mScrollPosition)
				return;

			mScrollPosition = pos;
		}
		else
		{
			int start = mPreActionOffset.left + (_left - point.left);
			if (start < (int)mSkinRangeStart)
				start = (int)mSkinRangeStart;
			else if (start > (getTrackPlaceLength() - (int)mSkinRangeEnd - mWidgetTrack->getWidth()))
				start = (getTrackPlaceLength() - (int)mSkinRangeEnd - mWidgetTrack->getWidth());
			if (mWidgetTrack->getLeft() != start)
				mWidgetTrack->setPosition(IntPoint(start, mWidgetTrack->getTop()));

			int pos = 0;
			if (mScrollRange >= 2)
			{
				pos = start - (int)mSkinRangeStart + (getLineSize() - getTrackSize()) / (((int)mScrollRange - 1) * 2);
				pos = pos * (int)(mScrollRange - 1) / (getLineSize() - getTrackSize());
			}

			if (pos < 0)
				pos = 0;
			else if (pos >= (int)mScrollRange)
				pos = (int)mScrollRange - 1;
			if (pos == (int)mScrollPosition)
				return;

			mScrollPosition = pos;
		}

		updateTrack();

		eventScrollChangePosition(this, (int)mScrollPosition);
	}

	void ScrollBar::onMouseButtonPressedRouted(MouseButtonEvent& _event)
	{
		if (_event.getButton() != MouseButton::Left)
			return;

		auto* part = _event.getTarget();
		if (part == mWidgetStart || part == mWidgetEnd || part == mWidgetTrack || part == mWidgetFirstPart ||
			part == mWidgetSecondPart)
			pressScrollPart(part);
	}

	void ScrollBar::notifyMousePressed(Widget* _sender, int _left, int _top, MouseButton _id)
	{
		if (_id == MouseButton::Left)
			pressScrollPart(_sender);
	}

	void ScrollBar::pressScrollPart(Widget* _part)
	{
		beginMouseInteraction(_part, MouseButton::Left);
		if (_part == mWidgetTrack)
		{
			mPreActionOffset = _part->getPosition();
			mDraggingTrack = true;
			return;
		}

		if (mMoveToClick && _part != mWidgetStart && _part != mWidgetEnd)
		{
			if (mWidgetTrack != nullptr)
			{
				auto& input = InputManager::getInstance();
				mPreActionOffset = input.getLastPressedPositionForWidget(MouseButton::Left, _part);
				const IntPoint point =
					input.getMousePositionForWidget(_part) - mWidgetTrack->getParent()->getAbsolutePosition();
				mPreActionOffset.left -= getTrackSize() / 2;
				mPreActionOffset.top -= getTrackSize() / 2;
				TrackMove(point.left, point.top);
			}
			return;
		}

		if (mEnableRepeat)
		{
			auto& controllers = ControllerManager::getInstance();
			auto* controller =
				controllers.createItem(ControllerRepeatClick::getClassTypeName())->castType<ControllerRepeatClick>();
			controller->eventRepeatClick += newDelegate(this, &ScrollBar::repeatClick);
			controller->setRepeat(mRepeatTriggerTime, mRepeatStepTime);
			controllers.addItem(_part, controller);
		}
		stepByPart(_part);
	}

	void ScrollBar::onMouseInteractionEnd(Widget* _target, MouseButton _button)
	{
		if (_button == MouseButton::Left)
			mDraggingTrack = false;
		ControllerManager::getInstance().removeItem(_target);
		updateTrack();
	}

	void ScrollBar::notifyMouseReleased(Widget* _sender, int _left, int _top, MouseButton _id)
	{
		onMouseInteractionEnd(_sender, _id);
	}

	void ScrollBar::notifyMouseDrag(Widget* _sender, int _left, int _top, MouseButton _id)
	{
		if (mDraggingTrack && _id == MouseButton::Left && mScrollRange >= 2)
			TrackMove(_left, _top);
	}

	void ScrollBar::setScrollRange(size_t _range)
	{
		if (_range == mScrollRange)
			return;

		mScrollRange = _range;
		mScrollPosition = (mScrollPosition < mScrollRange) ? mScrollPosition : 0;
		updateTrack();
	}

	void ScrollBar::setScrollPosition(size_t _position)
	{
		if (_position == mScrollPosition)
			return;

		if (_position >= mScrollRange)
			_position = 0;

		mScrollPosition = _position;
		updateTrack();
	}

	void ScrollBar::setPosition(const IntPoint& _point)
	{
		Base::setPosition(_point);
	}

	void ScrollBar::setSize(const IntSize& _size)
	{
		Base::setSize(_size);
		updateTrack();
	}

	void ScrollBar::setCoord(const IntCoord& _coord)
	{
		Base::setCoord(_coord);
		updateTrack();
	}

	void ScrollBar::setTrackSize(int _size)
	{
		if (mWidgetTrack != nullptr)
		{
			if (mVerticalAlignment)
				mWidgetTrack->setSize(
					mWidgetTrack->getWidth(),
					((int)_size < (int)mMinTrackSize) ? (int)mMinTrackSize : (int)_size);
			else
				mWidgetTrack->setSize(
					((int)_size < (int)mMinTrackSize) ? (int)mMinTrackSize : (int)_size,
					mWidgetTrack->getHeight());
		}
		updateTrack();
	}

	void ScrollBar::setRepeatTriggerTime(float time)
	{
		mRepeatTriggerTime = time;
	}

	void ScrollBar::setRepeatStepTime(float time)
	{
		mRepeatStepTime = time;
	}

	float ScrollBar::getRepeatTriggerTime(float time) const
	{
		return mRepeatTriggerTime;
	}

	float ScrollBar::getRepeatStepTime(float time) const
	{
		return mRepeatStepTime;
	}

	void ScrollBar::setRepeatEnabled(bool enabled)
	{
		mEnableRepeat = enabled;
	}

	bool ScrollBar::getRepeatEnabled() const
	{
		return mEnableRepeat;
	}

	int ScrollBar::getTrackSize() const
	{
		if (mWidgetTrack != nullptr)
		{
			if (mVerticalAlignment)
				return mWidgetTrack->getHeight();
			return mWidgetTrack->getWidth();
		}
		return 1;
	}

	int ScrollBar::getLineSize() const
	{
		return getTrackPlaceLength() - (int)(mSkinRangeStart + mSkinRangeEnd);
	}

	void ScrollBar::onMouseWheelRouted(MouseWheelEvent& _event)
	{
		const int rel = _event.getWheelDelta();
		if (rel == 0 || mScrollRange < 2)
			return;

		int offset = mScrollPosition;
		if (rel < 0)
			offset += mScrollWheelPage;
		else
			offset -= mScrollWheelPage;

		if (offset < 0)
			offset = 0;
		else if (offset > (int)(mScrollRange - 1))
			offset = mScrollRange - 1;

		if ((size_t)offset != mScrollPosition)
		{
			_event.setHandled();
			mScrollPosition = offset;
			eventScrollChangePosition(this, (int)mScrollPosition);
			updateTrack();
		}
	}

	void ScrollBar::repeatClick(Widget* _widget, ControllerItem* _controller)
	{
		stepByPart(_widget);
	}

	void ScrollBar::stepByPart(Widget* _widget)
	{
		if (_widget == mWidgetStart)
			widgetStartPressed();
		else if (_widget == mWidgetEnd)
			widgetEndPressed();
		else if (_widget == mWidgetFirstPart)
			widgetFirstPartPressed();
		else if (_widget == mWidgetSecondPart)
			widgetSecondPartPressed();
	}

	void ScrollBar::widgetStartPressed()
	{
		if (mScrollPosition == 0)
			return;

		if (mScrollPosition > mScrollPage)
			mScrollPosition -= mScrollPage;
		else
			mScrollPosition = 0;

		eventScrollChangePosition(this, (int)mScrollPosition);
		updateTrack();
	}

	void ScrollBar::widgetEndPressed()
	{
		if ((mScrollRange < 2) || (mScrollPosition >= (mScrollRange - 1)))
			return;

		if ((mScrollPosition + mScrollPage) < (mScrollRange - 1))
			mScrollPosition += mScrollPage;
		else
			mScrollPosition = mScrollRange - 1;

		eventScrollChangePosition(this, (int)mScrollPosition);
		updateTrack();
	}

	void ScrollBar::widgetFirstPartPressed()
	{
		if (mScrollPosition == 0)
			return;

		if (mScrollPosition > mScrollViewPage)
			mScrollPosition -= mScrollViewPage;
		else
			mScrollPosition = 0;

		eventScrollChangePosition(this, (int)mScrollPosition);
		updateTrack();
	}

	void ScrollBar::widgetSecondPartPressed()
	{
		if ((mScrollRange < 2) || (mScrollPosition >= (mScrollRange - 1)))
			return;

		if ((mScrollPosition + mScrollViewPage) < (mScrollRange - 1))
			mScrollPosition += mScrollViewPage;
		else
			mScrollPosition = mScrollRange - 1;

		eventScrollChangePosition(this, (int)mScrollPosition);
		updateTrack();
	}

	void ScrollBar::setPropertyOverride(std::string_view _key, std::string_view _value)
	{
		/// @wproperty{ScrollBar, Range, size_t} Scroll range.
		if (_key == "Range")
			setScrollRange(utility::parseValue<size_t>(_value));

		/// @wproperty{ScrollBar, RangePosition, size_t} Scroll position.
		else if (_key == "RangePosition")
			setScrollPosition(utility::parseValue<size_t>(_value));

		/// @wproperty{ScrollBar, Page, size_t} Scroll step when pressing start or end button.
		else if (_key == "Page")
			setScrollPage(utility::parseValue<size_t>(_value));

		/// @wproperty{ScrollBar, ViewPage, size_t} Scroll step when pressing one of the parts from button to track.
		else if (_key == "ViewPage")
			setScrollViewPage(utility::parseValue<size_t>(_value));

		/// @wproperty{ScrollBar, WheelPage, size_t} Scroll step when using mouse wheel.
		else if (_key == "WheelPage")
			setScrollWheelPage(utility::parseValue<size_t>(_value));

		/// @wproperty{ScrollBar, MoveToClick, bool} Move track to click position mode.
		else if (_key == "MoveToClick")
			setMoveToClick(utility::parseValue<bool>(_value));

		/// @wproperty{ScrollBar, VerticalAlignment, bool} Vertical alignment.
		else if (_key == "VerticalAlignment")
			setVerticalAlignment(utility::parseValue<bool>(_value));

		/// @wproperty{ScrollBar, Repeat, bool} Sets whether scrollbar buttons should be triggered repeatedly so long as the mouse button is pressed down.
		else if (_key == "Repeat")
			setRepeatEnabled(utility::parseValue<bool>(_value));

		/// @wproperty{ScrollBar, RepeatTriggerTime, float} How long the mouse needs to be pressed on a scrollbar button for repeating to start.
		else if (_key == "RepeatTriggerTime")
			setRepeatTriggerTime(utility::parseValue<float>(_value));

		/// @wproperty{ScrollBar, RepeatStepTime, float) The time between each repeat step once repeating has started.
		else if (_key == "RepeatStepTime")
			setRepeatStepTime(utility::parseValue<float>(_value));

		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}

		eventChangeProperty(this, _key, _value);
	}

	size_t ScrollBar::getScrollRange() const
	{
		return mScrollRange;
	}

	size_t ScrollBar::getScrollPosition() const
	{
		return mScrollPosition;
	}

	void ScrollBar::setScrollPage(size_t _value)
	{
		mScrollPage = _value;
	}

	size_t ScrollBar::getScrollPage() const
	{
		return mScrollPage;
	}

	void ScrollBar::setScrollViewPage(size_t _value)
	{
		mScrollViewPage = _value;
	}

	size_t ScrollBar::getScrollViewPage() const
	{
		return mScrollViewPage;
	}

	void ScrollBar::setScrollWheelPage(size_t _value)
	{
		mScrollWheelPage = _value;
	}
	size_t ScrollBar::getScrollWheelPage() const
	{
		return mScrollWheelPage;
	}

	void ScrollBar::setMinTrackSize(int _value)
	{
		mMinTrackSize = _value;
	}

	int ScrollBar::getMinTrackSize() const
	{
		return mMinTrackSize;
	}

	void ScrollBar::setMoveToClick(bool _value)
	{
		mMoveToClick = _value;
	}

	bool ScrollBar::getMoveToClick() const
	{
		return mMoveToClick;
	}

	int ScrollBar::getTrackPlaceLength() const
	{
		if (mWidgetTrack != nullptr)
		{
			if (mVerticalAlignment)
				return mWidgetTrack->getParent()->getHeight();
			return mWidgetTrack->getParent()->getWidth();
		}
		return 0;
	}

	void ScrollBar::setVerticalAlignment(bool _value)
	{
		mVerticalAlignment = _value;

		updateTrack();
	}

	bool ScrollBar::getVerticalAlignment() const
	{
		return mVerticalAlignment;
	}

} // namespace MyGUI
