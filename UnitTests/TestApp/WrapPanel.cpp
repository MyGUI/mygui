/*!
	@file
	@author		Albert Semenov
	@date		02/2011
*/

#include "WrapPanel.h"
#include "MyGUI_TextBox.h"

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

	void WrapPanel::alignChildLine(size_t _startIndex, size_t _stopIndex, int _top, int _height)
	{
		int currentWidth = 0;
		for (size_t index = _startIndex; index < _stopIndex; ++ index)
		{
			Widget* child = getChildAt(index);
			IntSize size = Panel::getDesiredSize(child);

			Panel::updateArrange(child, IntCoord(currentWidth, _top + _height - size.height, size.width, size.height));
			currentWidth += size.width;
		}
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
				alignChildLine(startLineIndex, index, currentHeight, maxLineHeight);

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
			alignChildLine(startLineIndex, count, currentHeight, maxLineHeight);
	}

} // namespace MyGUI
