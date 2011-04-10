/*!
	@file
	@author		Albert Semenov
	@date		02/2011
*/

#include "WrapPanel.h"

namespace MyGUI
{

	WrapPanel::WrapPanel()
	{
	}

	IntSize WrapPanel::overrideMeasure(const IntSize& _sizeAvailable)
	{
		IntSize result;

		int maxWidth = _sizeAvailable.width;
		int maxLineHeight = 0;
		bool hasWidget = false;

		size_t count = getChildCount();
		for (size_t index = 0; index < count; ++ index)
		{
			Widget* child = getChildAt(index);
			Panel::updateMeasure(child, _sizeAvailable);
			IntSize size = Panel::getDesiredSize(child);

			if (((result.width + size.width) > maxWidth) && hasWidget)
			{
				result.height += maxLineHeight;
				result.width = size.width;
				maxLineHeight = size.height;
			}
			else
			{
				result.width += size.width;
				if (size.height > maxLineHeight)
					maxLineHeight = size.height;
			}

			hasWidget = true;
		}

		result.height += maxLineHeight;

		return result;
	}

	void WrapPanel::overrideArrange()
	{
		int currentWidth = 0;
		int currentHeight = 0;
		int maxWidth = getWidth();
		int maxLineHeight = 0;
		bool hasWidget = false;
		size_t startLineIndex = 0;

		size_t count = getChildCount();
		for (size_t index = 0; index < count; ++ index)
		{
			Widget* child = getChildAt(index);
			IntSize size = Panel::getDesiredSize(child);

			if (((currentWidth + size.width) > maxWidth) && hasWidget)
			{
				alignChildLine(startLineIndex, index, IntCoord(0, currentHeight, maxWidth, maxLineHeight), currentWidth);

				currentHeight += maxLineHeight;
				currentWidth = size.width;
				maxLineHeight = size.height;

				startLineIndex = index;
			}
			else
			{
				currentWidth += size.width;
				if (size.height > maxLineHeight)
					maxLineHeight = size.height;
			}

			hasWidget = true;
		}

		if (hasWidget)
			alignChildLine(startLineIndex, count, IntCoord(0, currentHeight, maxWidth, maxLineHeight), currentWidth);
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
			left += size.width;
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

} // namespace MyGUI
