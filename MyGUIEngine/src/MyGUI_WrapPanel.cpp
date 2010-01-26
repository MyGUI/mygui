/*!
	@file
	@author		Albert Semenov
	@date		01/2010
	@module
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
#include "MyGUI_WrapPanel.h"
#include "MyGUI_ResourceSkin.h"

namespace MyGUI
{

	WrapPanel::WrapPanel() :
		//mFlowDirection(FlowDirection::TopToBottom),
		//mUniform(false),
		mHorizontalSpacer(0),
		mVerticalSpacer(0)
	{
	}

	void WrapPanel::overrideMeasure(const IntSize& _sizeAvailable)
	{
		mDesiredSize.clear();
		IntSize size_max;
		IntSize size_place(_sizeAvailable.width - getPaddingWidth(), MAX_COORD/*_sizeAvailable.height - getPaddingHeight()*/);

		int current_width = 0;
		int current_height = 0;
		//int count = 0;

		EnumeratorWidgetPtr child = getEnumerator();
		while (child.next())
		{
			if (!child->isVisible())
				continue;

			child->updateMeasure(size_place);
			const IntSize& child_size = child->getDesiredSize();

			if ((current_width + child_size.width) > size_place.width)
			{
				current_width = 0;
				current_height += size_max.height + mVerticalSpacer;
				size_max.height = 0;
			}

			current_width += child_size.width;
			size_max.width = std::max(size_max.width, current_width);
			size_max.height = std::max(size_max.height, child_size.height);
			current_width += mHorizontalSpacer;

			/*current_width += child_size.width;
			current_height += child_size.height;
			mMaxItemSize.width = std::max(mMaxItemSize.width, child_size.width);
			count ++;*/
		}

		mDesiredSize.width = size_max.width;
		mDesiredSize.height = current_height + size_max.height;

		/*if (mFlowDirection == FlowDirection::LeftToRight || mFlowDirection == FlowDirection::RightToLeft)
		{
			if (mUniform)
				mDesiredSize.width = count * mMaxItemSize.width;
			else
				mDesiredSize.width = current_width;
			mDesiredSize.height = mMaxItemSize.height;
			if (count > 1)
				mDesiredSize.width += mSpacer * (count - 1);
		}
		else
		{
			mDesiredSize.width = mMaxItemSize.width;
			if (mUniform)
				mDesiredSize.height = count * mMaxItemSize.height;
			else
				mDesiredSize.height = current_height;
			if (count > 1)
				mDesiredSize.height += mSpacer * (count - 1);
		}*/
	}

	int WrapPanel::getMaxHeight(EnumeratorWidgetPtr _child, int _maxwidth)
	{
		int result = 0;
		int current_width = 0;

		while (_child.next())
		{
			if (!_child->isVisible())
				continue;

			const IntSize& child_size = _child->getDesiredSize();

			if ((current_width + child_size.width) > _maxwidth)
			{
				break;
			}
			result = std::max(result, child_size.height);
			current_width += child_size.width + mHorizontalSpacer;
		}

		return result;
	}

	void WrapPanel::overrideArrange(const IntSize& _sizeOld)
	{
		// иначе детей не будет видно при мануал
		if (mSizePolicy == SizePolicy::Manual)
		{
			Base::overrideArrange(_sizeOld);
			return;
		}

		IntCoord coord_place(mPadding.left, mPadding.top, mCoord.width - getPaddingWidth(), mCoord.height - getPaddingHeight());
		int current_width = 0;
		int current_height = 0;
		IntSize size_max;

		/*int offset = 0;

		if (mFlowDirection == FlowDirection::LeftToRight)
			offset = coord_place.left;
		else if (mFlowDirection == FlowDirection::RightToLeft)
			offset = coord_place.right();
		else if (mFlowDirection == FlowDirection::TopToBottom)
			offset = coord_place.top;
		else if (mFlowDirection == FlowDirection::BottomToTop)
			offset = coord_place.bottom();*/

		EnumeratorWidgetPtr child_save = getEnumerator();
		size_max.height = getMaxHeight(child_save, coord_place.width);

		EnumeratorWidgetPtr child = getEnumerator();
		while (child.next())
		{
			if (!child->isVisible())
				continue;

			const IntSize& child_size = child->getDesiredSize();

			if ((current_width + child_size.width) > coord_place.width)
			{
				current_width = 0;
				current_height += size_max.height + mVerticalSpacer;
				size_max.height = getMaxHeight(child_save, coord_place.width);
			}

			IntCoord coord(current_width + coord_place.left, current_height + coord_place.top, child_size.width, size_max.height);
			current_width += child_size.width + mHorizontalSpacer;

			//mMaxItemSize.height = std::max(mMaxItemSize.height, child_size.height);

			/*if (mFlowDirection == FlowDirection::LeftToRight)
			{
				int width = mUniform ? mMaxItemSize.width : child_size.width;
				coord.set(offset, coord_place.top, width, coord_place.height);
				offset += width;
				offset += mSpacer;
			}
			else if (mFlowDirection == FlowDirection::RightToLeft)
			{
				int width = mUniform ? mMaxItemSize.width : child_size.width;
				offset -= width;
				coord.set(offset, coord_place.top, width, coord_place.height);
				offset -= mSpacer;
			}
			else if (mFlowDirection == FlowDirection::TopToBottom)
			{
				int height = mUniform ? mMaxItemSize.height : child_size.height;
				coord.set(coord_place.left, offset, coord_place.width, height);
				offset += height;
				offset += mSpacer;
			}
			else if (mFlowDirection == FlowDirection::BottomToTop)
			{
				int height = mUniform ? mMaxItemSize.height : child_size.height;
				offset -= height;
				coord.set(coord_place.left, offset, coord_place.width, height);
				offset -= mSpacer;
			}*/

			child->updateArrange(coord, coord.size());
			child_save = child;
		}
	}

	/*void WrapPanel::setUniform(bool _value)
	{
		mUniform = _value;
		invalidateMeasure();
	}*/

	void WrapPanel::setHorizontalSpacer(int _value)
	{
		mHorizontalSpacer = _value;
		invalidateMeasure();
	}

	void WrapPanel::setVerticalSpacer(int _value)
	{
		mVerticalSpacer = _value;
		invalidateMeasure();
	}

	/*void WrapPanel::setFlowDirection(FlowDirection _value)
	{
		mFlowDirection = _value;
		invalidateMeasure();
	}*/

	void WrapPanel::setProperty(const std::string& _key, const std::string& _value)
	{
		if (_key == "StackPanel_HorizontalSpacer") setHorizontalSpacer(utility::parseValue<int>(_value));
		else if (_key == "StackPanel_VerticalSpacer") setVerticalSpacer(utility::parseValue<int>(_value));
		//else if (_key == "StackPanel_FlowDirection") setFlowDirection(utility::parseValue<FlowDirection>(_value));
		else
		{
			Base::setProperty(_key, _value);
			return;
		}
		eventChangeProperty(this, _key, _value);
	}

} // namespace MyGUI
