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
		//if (mContentFloat)
			return floatMeasure(_sizeAvailable);
		//return simpleMeasure(_sizeAvailable);
	}

	void WrapPanel::overrideArrange()
	{
		//if (mContentFloat)
			floatArrange();
		/*else
			simpleArrange();*/
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

	/*IntSize WrapPanel::simpleMeasure(const IntSize& _sizeAvailable)
	{
		IntSize result;

		IntCoord coordAvailable = IntCoord(0, 0, _sizeAvailable.width, _sizeAvailable.height);
		IntPoint currentPosition = coordAvailable.point();

		int maxLineHeight = 0;
		bool hasAnyWidget = false;

		size_t count = getChildCount();
		for (size_t index = 0; index < count; ++ index)
		{
			Widget* child = getChildAt(index);
			Panel::updateMeasure(child, _sizeAvailable);
			IntSize size = Panel::getDesiredSize(child);

			if (((currentPosition.left + size.width) > coordAvailable.width))
			{
				if (hasAnyWidget)
				{
					result.width = (std::max)(result.width, currentPosition.left - mSpacer.width);

					currentPosition.left = coordAvailable.left;
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

					currentPosition.left = coordAvailable.left;
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
	}*/

	IntSize WrapPanel::floatMeasure(const IntSize& _sizeAvailable)
	{
		IntSize result;

		IntCoord coordAvailable = IntCoord(0, 0, _sizeAvailable.width, _sizeAvailable.height);
		IntPoint currentPosition = coordAvailable.point();

		int maxLineHeight = 0;
		bool hasAnyWidget = false;

		size_t count = getChildCount();
		for (size_t index = 0; index < count; ++ index)
		{
			Widget* child = getChildAt(index);
			Panel::updateMeasure(child, _sizeAvailable);
			IntSize size = Panel::getDesiredSize(child);

			if (((currentPosition.left + size.width) > coordAvailable.width))
			{
				if (hasAnyWidget)
				{
					result.width = (std::max)(result.width, currentPosition.left - mSpacer.width);

					currentPosition.left = coordAvailable.left;
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

	void WrapPanel::floatArrange()
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

					currentPosition.left = coordAvailable.left;
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

} // namespace MyGUI
