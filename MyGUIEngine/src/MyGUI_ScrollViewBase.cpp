/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ScrollViewBase.h"
#include "MyGUI_ScrollBar.h"
#include <algorithm>

namespace MyGUI
{

	namespace
	{

		void updateScrollBar(ScrollBar* _scroll, int _viewSize, int _contentSize, size_t _range, size_t _page)
		{
			_scroll->setScrollPage(_page);
			_scroll->setScrollViewPage(std::max(static_cast<size_t>(std::max(0, _viewSize)), _page));
			_scroll->setScrollRange(_range + 1);
			if (_contentSize != 0)
				_scroll->setTrackSize(
					static_cast<int>(static_cast<double>(_scroll->getLineSize()) * _viewSize / _contentSize));
		}

	}

	void ScrollViewBase::updateScrollSize()
	{
		if (mScrollViewClient == nullptr)
			return;

		eraseContent();
		IntSize contentSize = getContentSize();
		IntSize viewSize = getViewSize();

		const auto refreshSizes = [&]
		{
			if (mChangeContentByResize)
			{
				eraseContent();
				contentSize = getContentSize();
			}
			viewSize = getViewSize();
		};

		const auto setVerticalVisible = [&](bool _visible)
		{
			if (mVScroll == nullptr || mVScroll->getVisible() == _visible)
				return false;

			const int delta = _visible ? -mVScroll->getWidth() : mVScroll->getWidth();
			mVScroll->setVisible(_visible);
			mScrollViewClient->setSize(mScrollViewClient->getWidth() + delta, mScrollViewClient->getHeight());
			if (mHScroll != nullptr)
				mHScroll->setSize(mHScroll->getWidth() + delta, mHScroll->getHeight());
			refreshSizes();
			return true;
		};

		const auto setHorizontalVisible = [&](bool _visible)
		{
			if (mHScroll == nullptr || mHScroll->getVisible() == _visible)
				return false;

			const int delta = _visible ? -mHScroll->getHeight() : mHScroll->getHeight();
			mHScroll->setVisible(_visible);
			mScrollViewClient->setSize(mScrollViewClient->getWidth(), mScrollViewClient->getHeight() + delta);
			if (mVScroll != nullptr)
				mVScroll->setSize(mVScroll->getWidth(), mVScroll->getHeight() + delta);
			refreshSizes();
			return true;
		};

		// Preserve the vertical-first layout pass and remeasure after each visibility change.
		// Showing a scrollbar can require the other one; hiding it can make the other unnecessary.
		const bool showVertical = mVisibleVScroll && contentSize.height > viewSize.height;
		if (setVerticalVisible(showVertical))
		{
			const bool showHorizontal = mVisibleHScroll && contentSize.width > viewSize.width;
			if (showHorizontal == showVertical)
				setHorizontalVisible(showHorizontal);
		}

		const bool showHorizontal = mVisibleHScroll && contentSize.width > viewSize.width;
		if (setHorizontalVisible(showHorizontal))
		{
			const bool showVerticalAfterResize = mVisibleVScroll && contentSize.height > viewSize.height;
			if (showVerticalAfterResize == showHorizontal)
				setVerticalVisible(showVerticalAfterResize);
		}

		mVRange = static_cast<size_t>(std::max(0, contentSize.height - viewSize.height));
		mHRange = static_cast<size_t>(std::max(0, contentSize.width - viewSize.width));

		if (mVScroll != nullptr)
			updateScrollBar(mVScroll, viewSize.height, contentSize.height, mVRange, getVScrollPage());
		if (mHScroll != nullptr)
			updateScrollBar(mHScroll, viewSize.width, contentSize.width, mHRange, getHScrollPage());
	}

	void ScrollViewBase::updateScrollPosition()
	{
		const IntSize contentSize = getContentSize();
		const IntPoint contentPoint = getContentPosition();
		IntPoint offset = contentPoint;

		const IntSize viewSize = getViewSize();

		const Align align = getContentAlign();

		if (contentSize.width > viewSize.width)
		{
			offset.left = std::clamp(offset.left, 0, contentSize.width - viewSize.width);
		}
		else
		{
			if (align.isLeft())
			{
				offset.left = 0;
			}
			else if (align.isRight())
			{
				offset.left = contentSize.width - viewSize.width;
			}
			else
			{
				offset.left = (contentSize.width - viewSize.width) / 2;
			}
		}

		if (contentSize.height > viewSize.height)
		{
			offset.top = std::clamp(offset.top, 0, contentSize.height - viewSize.height);
		}
		else
		{
			if (align.isTop())
			{
				offset.top = 0;
			}
			else if (align.isBottom())
			{
				offset.top = contentSize.height - viewSize.height;
			}
			else
			{
				offset.top = (contentSize.height - viewSize.height) / 2;
			}
		}

		if (offset != contentPoint)
		{
			if (mVScroll != nullptr)
				mVScroll->setScrollPosition(offset.top);
			if (mHScroll != nullptr)
				mHScroll->setScrollPosition(offset.left);
			setContentPosition(offset);
		}
	}

	IntSize ScrollViewBase::getContentSize() const
	{
		return {};
	}

	IntPoint ScrollViewBase::getContentPosition() const
	{
		return {};
	}

	void ScrollViewBase::setContentPosition(const IntPoint& _value)
	{
	}

	IntSize ScrollViewBase::getViewSize() const
	{
		return {};
	}

	size_t ScrollViewBase::getHScrollPage() const
	{
		return 1;
	}

	size_t ScrollViewBase::getVScrollPage() const
	{
		return 1;
	}

	Align ScrollViewBase::getContentAlign() const
	{
		return Align::Center;
	}

	void ScrollViewBase::eraseContent()
	{
	}

} // namespace MyGUI
