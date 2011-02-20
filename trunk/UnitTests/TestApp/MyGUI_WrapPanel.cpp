/*!
	@file
	@author		Albert Semenov
	@date		01/2010
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
		mHorizontalSpacer(0),
		mVerticalSpacer(0),
		mItemWidth(0),
		mItemHeight(0),
		mInvertWrap(false)
	{
	}

	IntSize WrapPanel::overrideMeasure(const IntSize& _sizeAvailable)
	{
		IntSize result;
		IntSize size_max;
		IntSize size_place(_sizeAvailable.width - getPadding(this).width(), _sizeAvailable.height - getPadding(this).height());
		if (mFlowDirection.isHorizontal())
			size_place.height = (std::numeric_limits<int>::max)();
		else
			size_place.width = (std::numeric_limits<int>::max)();

		int current_width = 0;
		int current_height = 0;

		EnumeratorWidgetPtr child = getEnumerator();
		while (child.next())
		{
			if (!child->getVisible())
				continue;

			if (mItemWidth != 0)
				size_place.width = std::min(size_place.width, mItemWidth - getPadding(this).width());
			if (mItemHeight != 0)
				size_place.height = std::min(size_place.height, mItemHeight - getPadding(this).height());

			updateMeasure(child.current(), size_place);
			IntSize child_size = getDesiredSize(child.current());

			if (mItemWidth != 0)
				child_size.width = mItemWidth;
			if (mItemHeight != 0)
				child_size.height = mItemHeight;

			if (mFlowDirection.isHorizontal())
			{
				if ((current_width + child_size.width) > size_place.width && current_width != 0)
				{
					current_width = 0;
					current_height += size_max.height + mVerticalSpacer;
					size_max.height = 0;
				}

				current_width += child_size.width;
				size_max.width = std::max(size_max.width, current_width);
				size_max.height = std::max(size_max.height, child_size.height);
				current_width += mHorizontalSpacer;
			}
			else if (mFlowDirection.isVertical())
			{
				if ((current_height + child_size.height) > size_place.height && current_height != 0)
				{
					current_height = 0;
					current_width += size_max.width + mHorizontalSpacer;
					size_max.width = 0;
				}

				current_height += child_size.height;
				size_max.height = std::max(size_max.height, current_height);
				size_max.width = std::max(size_max.width, child_size.width);
				current_height += mVerticalSpacer;
			}
		}

		if (mFlowDirection.isHorizontal())
		{
			result.width = size_max.width;
			result.height = current_height + size_max.height;
		}
		else if (mFlowDirection.isVertical())
		{
			result.width = current_width + size_max.width;
			result.height = size_max.height;
		}

		return result;
	}

	IntSize WrapPanel::getMaxDistance(EnumeratorWidgetPtr _child, const IntSize& _max)
	{
		IntSize result;
		int current_width = 0;
		int current_height = 0;

		while (_child.next())
		{
			if (!_child->getVisible())
				continue;

			IntSize child_size = getDesiredSize(_child.current());

			if (mItemWidth != 0)
				child_size.width = mItemWidth;
			if (mItemHeight != 0)
				child_size.height = mItemHeight;

			if (mFlowDirection.isHorizontal())
			{
				if ((current_width + child_size.width) > _max.width && current_width != 0)
				{
					break;
				}
				result.height = std::max(result.height, child_size.height);
				current_width += child_size.width + mHorizontalSpacer;
			}
			else if (mFlowDirection.isVertical())
			{
				if ((current_height + child_size.height) > _max.height && current_height != 0)
				{
					break;
				}
				result.width = std::max(result.width, child_size.width);
				current_height += child_size.height + mVerticalSpacer;
			}
		}

		return result;
	}

	void WrapPanel::overrideArrange(const IntSize& _sizeOld)
	{
		// ����� ����� �� ����� ����� ��� ������
		/*if (getSizePolicy() == SizePolicy::Manual)
		{
			Base::overrideArrange(_sizeOld);
			return;
		}*/

		IntCoord coord_place(getPadding(this).left, getPadding(this).top, mCoord.width - getPadding(this).width(), mCoord.height - getPadding(this).height());
		int current_width = 0;
		int current_height = 0;

		EnumeratorWidgetPtr child_save = getEnumerator();
		IntSize size_max = getMaxDistance(child_save, coord_place.size());

		EnumeratorWidgetPtr child = getEnumerator();
		while (child.next())
		{
			if (!child->getVisible())
				continue;

			IntSize child_size = getDesiredSize(child.current());

			if (mItemWidth != 0)
				child_size.width = mItemWidth;
			if (mItemHeight != 0)
				child_size.height = mItemHeight;

			IntCoord coord;

			if (mFlowDirection.isHorizontal())
			{
				if ((current_width + child_size.width) > coord_place.width && current_width != 0)
				{
					current_width = 0;
					current_height += size_max.height + mVerticalSpacer;
					size_max = getMaxDistance(child_save, coord_place.size());
				}

				coord.set(current_width + coord_place.left, current_height + coord_place.top, child_size.width, size_max.height);
				if (mFlowDirection == FlowDirection::RightToLeft)
					coord.left = coord_place.right() - current_width - child_size.width;
				if (mInvertWrap)
					coord.top = coord_place.bottom() - current_height - size_max.height;
				current_width += child_size.width + mHorizontalSpacer;
			}
			else if (mFlowDirection.isVertical())
			{
				if ((current_height + child_size.height) > coord_place.height && current_height != 0)
				{
					current_height = 0;
					current_width += size_max.width + mHorizontalSpacer;
					size_max = getMaxDistance(child_save, coord_place.size());
				}

				coord.set(current_width + coord_place.left, current_height + coord_place.top, size_max.width, child_size.height);
				if (mFlowDirection == FlowDirection::BottomToTop)
					coord.top = coord_place.bottom() - current_height - child_size.height;
				if (mInvertWrap)
					coord.left = coord_place.right() - current_width - size_max.width;
				current_height += child_size.height + mVerticalSpacer;
			}

			updateArrange(child.current(), coord, coord.size());
			child_save = child;
		}
	}

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

	void WrapPanel::setFlowDirection(FlowDirection _value)
	{
		mFlowDirection = _value;
		invalidateMeasure();
	}

	void WrapPanel::setItemWidth(int _value)
	{
		mItemWidth = _value;
		invalidateMeasure();
	}

	void WrapPanel::setItemHeight(int _value)
	{
		mItemHeight = _value;
		invalidateMeasure();
	}

	void WrapPanel::setInvertWrap(bool _value)
	{
		mInvertWrap = _value;
		invalidateMeasure();
	}

	void WrapPanel::setProperty(const std::string& _key, const std::string& _value)
	{
		if (_key == "StackPanel_HorizontalSpacer") setHorizontalSpacer(utility::parseValue<int>(_value));
		else if (_key == "StackPanel_VerticalSpacer") setVerticalSpacer(utility::parseValue<int>(_value));
		else if (_key == "StackPanel_ItemWidth") setItemWidth(utility::parseValue<int>(_value));
		else if (_key == "StackPanel_ItemHeight") setItemHeight(utility::parseValue<int>(_value));
		else if (_key == "StackPanel_FlowDirection") setFlowDirection(utility::parseValue<FlowDirection>(_value));
		else if (_key == "StackPanel_InvertWrap") setInvertWrap(utility::parseValue<bool>(_value));
		else
		{
			Base::setProperty(_key, _value);
			return;
		}
		eventChangeProperty(this, _key, _value);
	}

} // namespace MyGUI
