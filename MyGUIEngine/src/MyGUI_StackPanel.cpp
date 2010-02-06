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
#include "MyGUI_StackPanel.h"
#include "MyGUI_ResourceSkin.h"

namespace MyGUI
{

	StackPanel::StackPanel() :
		mFlowDirection(FlowDirection::TopToBottom),
		mUniform(false),
		mSpacer(0)
	{
	}

	IntSize StackPanel::overrideMeasure(const IntSize& _sizeAvailable)
	{
		IntSize result;
		mMaxItemSize.clear();
		IntSize size_place(_sizeAvailable.width - getPaddingWidth(), _sizeAvailable.height - getPaddingHeight());

		if (mFlowDirection.isHorizontal())
			size_place.width = MAX_COORD;
		else
			size_place.height = MAX_COORD;

		int current_width = 0;
		int current_height = 0;
		int count = 0;

		EnumeratorWidgetPtr child = getEnumerator();
		while (child.next())
		{
			if (!child->isVisible())
				continue;

			child->updateMeasure(size_place);
			const IntSize& child_size = child->getDesiredSize();

			current_width += child_size.width;
			current_height += child_size.height;
			mMaxItemSize.width = std::max(mMaxItemSize.width, child_size.width);
			mMaxItemSize.height = std::max(mMaxItemSize.height, child_size.height);
			count ++;
		}

		if (mFlowDirection.isHorizontal())
		{
			if (mUniform)
				result.width = count * mMaxItemSize.width;
			else
				result.width = current_width;
			result.height = mMaxItemSize.height;
			if (count > 1)
				result.width += mSpacer * (count - 1);
		}
		else
		{
			result.width = mMaxItemSize.width;
			if (mUniform)
				result.height = count * mMaxItemSize.height;
			else
				result.height = current_height;
			if (count > 1)
				result.height += mSpacer * (count - 1);
		}

		return result;
	}

	void StackPanel::overrideArrange(const IntSize& _sizeOld)
	{
		// иначе детей не будет видно при мануал
		if (getSizePolicy() == SizePolicy::Manual)
		{
			Base::overrideArrange(_sizeOld);
			return;
		}

		IntCoord coord_place(getPadding().left, getPadding().top, mCoord.width - getPaddingWidth(), mCoord.height - getPaddingHeight());
		int offset = 0;

		if (mFlowDirection == FlowDirection::LeftToRight)
			offset = coord_place.left;
		else if (mFlowDirection == FlowDirection::RightToLeft)
			offset = coord_place.right();
		else if (mFlowDirection == FlowDirection::TopToBottom)
			offset = coord_place.top;
		else if (mFlowDirection == FlowDirection::BottomToTop)
			offset = coord_place.bottom();

		EnumeratorWidgetPtr child = getEnumerator();
		while (child.next())
		{
			if (!child->isVisible())
				continue;

			const IntSize& child_size = child->getDesiredSize();
			IntCoord coord;

			if (mFlowDirection == FlowDirection::LeftToRight)
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
			}

			child->updateArrange(coord, coord.size());
		}

	}

	void StackPanel::setUniform(bool _value)
	{
		mUniform = _value;
		invalidateMeasure();
	}

	void StackPanel::setSpacer(int _value)
	{
		mSpacer = _value;
		invalidateMeasure();
	}

	void StackPanel::setFlowDirection(FlowDirection _value)
	{
		mFlowDirection = _value;
		invalidateMeasure();
	}

	void StackPanel::setProperty(const std::string& _key, const std::string& _value)
	{
		if (_key == "StackPanel_Uniform") setUniform(utility::parseValue<bool>(_value));
		else if (_key == "StackPanel_Spacer") setSpacer(utility::parseValue<int>(_value));
		else if (_key == "StackPanel_FlowDirection") setFlowDirection(utility::parseValue<FlowDirection>(_value));
		else
		{
			Base::setProperty(_key, _value);
			return;
		}
		eventChangeProperty(this, _key, _value);
	}

} // namespace MyGUI
