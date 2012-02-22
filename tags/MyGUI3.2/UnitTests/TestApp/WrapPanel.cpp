/*!
	@file
	@author		Albert Semenov
	@date		02/2011
*/

#include "WrapPanel.h"

namespace MyGUI
{

	WrapPanel::WrapPanel() :
		mContentFloat(false)
	{
	}

	IntSize WrapPanel::overrideMeasure(const IntSize& _sizeAvailable)
	{
		if (mContentFloat)
			return floatMeasure(_sizeAvailable);
		return simpleMeasure(_sizeAvailable);
	}

	void WrapPanel::overrideArrange()
	{
		if (mContentFloat)
			floatArrange();
		else
			simpleArrange();
	}

	void WrapPanel::alignChildLine(size_t _startIndex, size_t _stopIndex, const IntCoord& _coordAvailable, int _lineWidth)
	{
		int left = _coordAvailable.left;
		if (mContentAlign.isHCenter())
			left = _coordAvailable.left + ((_coordAvailable.width - _lineWidth) / 2);
		else if (mContentAlign.isRight())
			left = _coordAvailable.left + (_coordAvailable.width - _lineWidth);

		for (size_t index = _startIndex; index < _stopIndex; ++ index)
		{
			Widget* child = getChildAt(index);
			IntSize size = Panel::getDesiredSize(child);

			int top = _coordAvailable.top;
			if (mContentAlign.isVCenter())
				top = _coordAvailable.top + ((_coordAvailable.height - size.height) / 2);
			else if (mContentAlign.isBottom())
				top = _coordAvailable.top + (_coordAvailable.height - size.height);

			Panel::updateArrange(child, IntCoord(left, top, size.width, size.height));
			left += size.width + mSpacer.width;
		}
	}

	Align WrapPanel::getContentAlign() const
	{
		return mContentAlign;
	}

	void WrapPanel::setContentAlign(Align _value)
	{
		mContentAlign = _value;
	}

	const IntSize& WrapPanel::getSpacer() const
	{
		return mSpacer;
	}

	void WrapPanel::setSpacer(const IntSize& _value)
	{
		mSpacer = _value;
	}

	bool WrapPanel::getContentFloat() const
	{
		return mContentFloat;
	}

	void WrapPanel::setContentFloat(bool _value)
	{
		mContentFloat = _value;
	}

	Align WrapPanel::getSnapFloat() const
	{
		return mSnapFloat;
	}

	void WrapPanel::setSnapFloat(Align _value)
	{
		mSnapFloat = _value;
	}

	IntSize WrapPanel::simpleMeasure(const IntSize& _sizeAvailable)
	{
		IntSize result;

		IntSize sizeAvailable = _sizeAvailable;
		IntPoint currentPosition;

		int maxLineHeight = 0;
		bool hasAnyWidget = false;

		size_t count = getChildCount();
		for (size_t index = 0; index < count; ++ index)
		{
			Widget* child = getChildAt(index);
			Panel::updateMeasure(child, sizeAvailable);
			IntSize size = Panel::getDesiredSize(child);

			if (((currentPosition.left + size.width) > sizeAvailable.width))
			{
				if (hasAnyWidget)
				{
					result.width = (std::max)(result.width, currentPosition.left - mSpacer.width);

					currentPosition.left = 0;
					currentPosition.top += maxLineHeight + mSpacer.height;
					maxLineHeight = 0;
				}
			}

			currentPosition.left += size.width + mSpacer.width;
			maxLineHeight = (std::max)(maxLineHeight, size.height);

			hasAnyWidget = true;
		}

		result.height = currentPosition.top + maxLineHeight;
		result.width = (std::max)(result.width, currentPosition.left);

		return result;
	}

	void WrapPanel::simpleArrange()
	{
		IntCoord coordAvailable(0, 0, getWidth(), getHeight());
		IntPoint currentPosition = coordAvailable.point();

		int maxLineHeight = 0;
		bool hasAnyWidget = false;
		size_t startLineIndex = 0;

		size_t count = getChildCount();
		for (size_t index = 0; index < count; ++ index)
		{
			Widget* child = getChildAt(index);
			IntSize size = Panel::getDesiredSize(child);

			if (((currentPosition.left + size.width) > coordAvailable.width))
			{
				if (hasAnyWidget)
				{
					alignChildLine(startLineIndex, index, IntCoord(coordAvailable.left, currentPosition.top, coordAvailable.width, maxLineHeight), currentPosition.left - mSpacer.width);

					currentPosition.left = 0;
					currentPosition.top += maxLineHeight + mSpacer.height;
					maxLineHeight = 0;

					startLineIndex = index;
				}
			}

			currentPosition.left += size.width + mSpacer.width;
			maxLineHeight = (std::max)(size.height, maxLineHeight);

			hasAnyWidget = true;
		}

		if (startLineIndex < count)
			alignChildLine(startLineIndex, count, IntCoord(coordAvailable.left, currentPosition.top, coordAvailable.width, maxLineHeight), currentPosition.left - mSpacer.width);
	}

	IntSize WrapPanel::floatMeasure(const IntSize& _sizeAvailable)
	{
		IntSize result;
		size_t count = getChildCount();

		if (count == 0)
			return result;

		Widget* child = getChildAt(0);
		Panel::updateMeasure(child, _sizeAvailable);
		IntSize firstSize = Panel::getDesiredSize(child);

		if (count == 1)
			return firstSize;

		IntSize sizeAvailable = _sizeAvailable;
		IntPoint currentPosition;

		int maxLineHeight = 0;
		bool hasAnyWidget = false;

		for (size_t index = 1; index < count; ++ index)
		{
			bool floatBehaivoir = ((firstSize.height + mSpacer.height) > currentPosition.top);

			if (floatBehaivoir)
			{
				sizeAvailable.width = _sizeAvailable.width - (firstSize.width + mSpacer.width);
			}
			else
			{
				sizeAvailable.width = _sizeAvailable.width;
			}

			Widget* child = getChildAt(index);
			Panel::updateMeasure(child, sizeAvailable);
			IntSize size = Panel::getDesiredSize(child);

			if ((currentPosition.left + size.width) > sizeAvailable.width)
			{
				if (hasAnyWidget || floatBehaivoir)
				{
					result.width = (std::max)(result.width, currentPosition.left - mSpacer.width);

					currentPosition.left = 0;
					currentPosition.top += maxLineHeight + mSpacer.height;
					maxLineHeight = 0;
				}
			}

			if (floatBehaivoir)
			{
				if (((currentPosition.left + size.width) > sizeAvailable.width))
				{
					-- index;
					continue;
				}
			}

			currentPosition.left += size.width + mSpacer.width;
			maxLineHeight = (std::max)(maxLineHeight, size.height);

			hasAnyWidget = true;
		}

		result.height = (std::max)(firstSize.height, currentPosition.top + maxLineHeight);
		result.width = (std::max)(result.width, currentPosition.left);

		return result;
	}

	void WrapPanel::floatArrange()
	{
		size_t count = getChildCount();

		if (count == 0)
			return;

		Widget* child = getChildAt(0);
		IntSize firstSize = Panel::getDesiredSize(child);
		if (mSnapFloat.isRight())
			Panel::updateArrange(child, IntCoord(getWidth() - firstSize.width, 0, firstSize.width, firstSize.height));
		else
			Panel::updateArrange(child, IntCoord(0, 0, firstSize.width, firstSize.height));

		if (count == 1)
			return;

		IntCoord coordAvailable(0, 0, getWidth(), getHeight());
		IntPoint currentPosition;

		int maxLineHeight = 0;
		bool hasAnyWidget = false;
		size_t startLineIndex = 1;

		for (size_t index = 1; index < count; ++ index)
		{
			bool floatBehaivoir = ((firstSize.height + mSpacer.height) > currentPosition.top);

			if (floatBehaivoir)
			{
				if (mSnapFloat.isRight())
					coordAvailable.left = 0;
				else
					coordAvailable.left = firstSize.width + mSpacer.width;
				coordAvailable.width = getWidth() - (firstSize.width + mSpacer.width);
			}
			else
			{
				coordAvailable.left = 0;
				coordAvailable.width = getWidth();
			}

			Widget* child = getChildAt(index);
			IntSize size = Panel::getDesiredSize(child);

			if ((currentPosition.left + size.width) > coordAvailable.width)
			{
				if (hasAnyWidget || floatBehaivoir)
				{
					alignChildLine(startLineIndex, index, IntCoord(coordAvailable.left, currentPosition.top, coordAvailable.width, maxLineHeight), currentPosition.left - mSpacer.width);

					currentPosition.left = 0;
					currentPosition.top += maxLineHeight + mSpacer.height;
					maxLineHeight = 0;

					startLineIndex = index;
				}
			}

			if (floatBehaivoir)
			{
				if (((currentPosition.left + size.width) > coordAvailable.width))
				{
					-- index;
					continue;
				}
			}

			currentPosition.left += size.width + mSpacer.width;
			maxLineHeight = (std::max)(size.height, maxLineHeight);

			hasAnyWidget = true;
		}

		if (startLineIndex < count)
			alignChildLine(startLineIndex, count, IntCoord(coordAvailable.left, currentPosition.top, coordAvailable.width, maxLineHeight), currentPosition.left - mSpacer.width);
	}

} // namespace MyGUI
