/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ScrollViewBase.h"
#include "MyGUI_ScrollBar.h"

namespace MyGUI
{

	void ScrollViewBase::updateScrollSize()
	{
		if (mScrollViewClient == nullptr)
			return;

		eraseContent();
		IntSize contentSize = getContentSize();
		IntSize viewSize = getViewSize();

		// вертикальный контент не помещается
		if (contentSize.height > viewSize.height)
		{
			if (mVScroll != nullptr)
			{
				if (!mVScroll->getVisible() && mVisibleVScroll)
				{
					mVScroll->setVisible(true);
					mScrollViewClient->setSize(
						mScrollViewClient->getWidth() - mVScroll->getWidth(),
						mScrollViewClient->getHeight());

					// размер может измениться
					if (mChangeContentByResize)
					{
						eraseContent();
						contentSize = getContentSize();
						viewSize = getViewSize();
					}

					if (mHScroll != nullptr)
					{
						mHScroll->setSize(mHScroll->getWidth() - mVScroll->getWidth(), mHScroll->getHeight());

						// если показали вертикальный скрол бар, уменьшилось вью по горизонтали,
						// пересчитываем горизонтальный скрол на предмет показа
						if ((contentSize.width > viewSize.width) && (!mHScroll->getVisible()) && (mVisibleHScroll))
						{
							mHScroll->setVisible(true);
							mScrollViewClient->setSize(
								mScrollViewClient->getWidth(),
								mScrollViewClient->getHeight() - mHScroll->getHeight());
							mVScroll->setSize(mVScroll->getWidth(), mVScroll->getHeight() - mHScroll->getHeight());

							// размер может измениться
							if (mChangeContentByResize)
							{
								eraseContent();
								contentSize = getContentSize();
								viewSize = getViewSize();
							}
						}
					}
				}
			}
		}
		// вертикальный контент помещается
		else
		{
			if (mVScroll != nullptr)
			{
				if (mVScroll->getVisible())
				{
					mVScroll->setVisible(false);
					mScrollViewClient->setSize(
						mScrollViewClient->getWidth() + mVScroll->getWidth(),
						mScrollViewClient->getHeight());

					// размер может измениться
					if (mChangeContentByResize)
					{
						eraseContent();
						contentSize = getContentSize();
						viewSize = getViewSize();
					}

					if (mHScroll != nullptr)
					{
						mHScroll->setSize(mHScroll->getWidth() + mVScroll->getWidth(), mHScroll->getHeight());

						// если скрыли вертикальный скрол бар, увеличилось вью по горизонтали,
						// пересчитываем горизонтальный скрол на предмет скрытия
						if ((contentSize.width <= viewSize.width) && (mHScroll->getVisible()))
						{
							mHScroll->setVisible(false);
							mScrollViewClient->setSize(
								mScrollViewClient->getWidth(),
								mScrollViewClient->getHeight() + mHScroll->getHeight());
							mVScroll->setSize(mVScroll->getWidth(), mVScroll->getHeight() + mHScroll->getHeight());

							// размер может измениться
							if (mChangeContentByResize)
							{
								eraseContent();
								contentSize = getContentSize();
								viewSize = getViewSize();
							}
						}
					}
				}
			}
		}


		// горизонтальный контент не помещается
		if (contentSize.width > viewSize.width)
		{
			if (mHScroll != nullptr)
			{
				if (!mHScroll->getVisible() && mVisibleHScroll)
				{
					mHScroll->setVisible(true);
					mScrollViewClient->setSize(
						mScrollViewClient->getWidth(),
						mScrollViewClient->getHeight() - mHScroll->getHeight());

					// размер может измениться
					if (mChangeContentByResize)
					{
						eraseContent();
						contentSize = getContentSize();
						viewSize = getViewSize();
					}

					if (mVScroll != nullptr)
					{
						mVScroll->setSize(mVScroll->getWidth(), mVScroll->getHeight() - mHScroll->getHeight());

						// если показали горизонтальный скрол бар, уменьшилось вью по вертикали,
						// пересчитываем вертикальный скрол на предмет показа
						if ((contentSize.height > viewSize.height) && (!mVScroll->getVisible()) && (mVisibleVScroll))
						{
							mVScroll->setVisible(true);
							mScrollViewClient->setSize(
								mScrollViewClient->getWidth() - mVScroll->getWidth(),
								mScrollViewClient->getHeight());
							mHScroll->setSize(mHScroll->getWidth() - mVScroll->getWidth(), mHScroll->getHeight());

							// размер может измениться
							if (mChangeContentByResize)
							{
								eraseContent();
								contentSize = getContentSize();
								viewSize = getViewSize();
							}
						}
					}
				}
			}
		}
		// горизонтальный контент помещается
		else
		{
			if (mHScroll != nullptr)
			{
				if (mHScroll->getVisible())
				{
					mHScroll->setVisible(false);
					mScrollViewClient->setSize(
						mScrollViewClient->getWidth(),
						mScrollViewClient->getHeight() + mHScroll->getHeight());

					// размер может измениться
					if (mChangeContentByResize)
					{
						eraseContent();
						contentSize = getContentSize();
						viewSize = getViewSize();
					}

					if (mVScroll != nullptr)
					{
						mVScroll->setSize(mVScroll->getWidth(), mVScroll->getHeight() + mHScroll->getHeight());

						// если скрыли горизонтальный скрол бар, увеличилось вью по вертикали,
						// пересчитываем вертикальный скрол на предмет скрытия
						if ((contentSize.height <= viewSize.height) && (mVScroll->getVisible()))
						{
							mVScroll->setVisible(false);
							mScrollViewClient->setSize(
								mScrollViewClient->getWidth() + mVScroll->getWidth(),
								mScrollViewClient->getHeight());
							mHScroll->setSize(mHScroll->getWidth() + mVScroll->getWidth(), mHScroll->getHeight());

							// размер может измениться
							if (mChangeContentByResize)
							{
								eraseContent();
								contentSize = getContentSize();
								viewSize = getViewSize();
							}
						}
					}
				}
			}
		}

		mVRange = (viewSize.height >= contentSize.height) ? 0 : contentSize.height - viewSize.height;
		mHRange = (viewSize.width >= contentSize.width) ? 0 : contentSize.width - viewSize.width;

		if (mVScroll != nullptr)
		{
			size_t page = getVScrollPage();
			mVScroll->setScrollPage(page);
			mVScroll->setScrollViewPage(viewSize.height > (int)page ? viewSize.height : page);
			mVScroll->setScrollRange(mVRange + 1);
			if (contentSize.height)
				mVScroll->setTrackSize(
					int(float(mVScroll->getLineSize() * viewSize.height) / float(contentSize.height)));
		}
		if (mHScroll != nullptr)
		{
			size_t page = getHScrollPage();
			mHScroll->setScrollPage(page);
			mHScroll->setScrollViewPage(viewSize.width > (int)page ? viewSize.width : page);
			mHScroll->setScrollRange(mHRange + 1);
			if (contentSize.width)
				mHScroll->setTrackSize(int(float(mHScroll->getLineSize() * viewSize.width) / float(contentSize.width)));
		}
	}

	void ScrollViewBase::updateScrollPosition()
	{
		// размер контекста
		IntSize contentSize = getContentSize();
		// текущее смещение контекста
		IntPoint contentPoint = getContentPosition();
		// расчетное смещение
		IntPoint offset = contentPoint;

		IntSize viewSize = getViewSize();

		Align align = getContentAlign();

		if (contentSize.width > viewSize.width)
		{
			// максимальный выход влево
			if ((offset.left + viewSize.width) > contentSize.width)
			{
				offset.left = contentSize.width - viewSize.width;
			}
			// максимальный выход вправо
			else if (offset.left < 0)
			{
				offset.left = 0;
			}
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
			// максимальный выход вверх
			if ((offset.top + viewSize.height) > contentSize.height)
			{
				offset.top = contentSize.height - viewSize.height;
			}
			// максимальный выход вниз
			else if (offset.top < 0)
			{
				offset.top = 0;
			}
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
			if (nullptr != mVScroll)
				mVScroll->setScrollPosition(offset.top);
			if (nullptr != mHScroll)
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
