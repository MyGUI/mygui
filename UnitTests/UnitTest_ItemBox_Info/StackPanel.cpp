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
#include "StackPanel.h"
#include "MyGUI_ResourceSkin.h"

namespace MyGUI
{

	StackPanel::StackPanel() :
		mFlowToDirection(Align::Bottom),
		mUniform(false),
		mSpacer(0)
	{
	}

	const IntSize& StackPanel::updateMeasure(const IntSize& _sizeAvailable)
	{
		int count = 0;
		mMaxItemSize.clear();
		mDesiredSize.clear();

		EnumeratorWidgetPtr child = getEnumerator();
		while (child.next())
		{
			if (!child->isVisible())
				continue;

			child->setMeasure(_sizeAvailable);
			const IntSize& child_size = child->getDesiredSize();

			mMaxItemSize.width = std::max(mMaxItemSize.width, child_size.width);
			mMaxItemSize.height = std::max(mMaxItemSize.height, child_size.height);
			count ++;

			mDesiredSize.width += child_size.width;
			mDesiredSize.height += child_size.height;
		}

		if (mUniform)
		{
			if (mFlowToDirection == Align::Bottom || mFlowToDirection == Align::Top)
			{
				mDesiredSize.width = mMaxItemSize.width;
				mDesiredSize.height = mMaxItemSize.height * count;
			}
			else
			{
				mDesiredSize.height = mMaxItemSize.height;
				mDesiredSize.width = mMaxItemSize.width * count;
			}
		}
		else
		{
			if (mFlowToDirection == Align::Bottom || mFlowToDirection == Align::Top)
			{
				mDesiredSize.width = mMaxItemSize.width;
			}
			else
			{
				mDesiredSize.height = mMaxItemSize.height;
			}
		}

		if (count > 1)
		{
			if (mFlowToDirection == Align::Bottom || mFlowToDirection == Align::Top)
				mDesiredSize.height += mSpacer * (count - 1);
			else
				mDesiredSize.width += mSpacer * (count - 1);
		}

		return mDesiredSize;
	}

	void StackPanel::updateArrange(const IntSize& _sizeFinal)
	{
		int offset = 0;
		int item_offset = 0;
		int positiv_diff = 0;
		int step_coeef = 1;

		if (mFlowToDirection == Align::Bottom || mFlowToDirection == Align::Top)
			positiv_diff = std::max(0, _sizeFinal.height - mDesiredSize.height - (mThickness.top + mThickness.bottom));
		else
			positiv_diff = std::max(0, _sizeFinal.width - mDesiredSize.width - (mThickness.left + mThickness.right));

		if (mFlowToDirection == Align::Left)
		{
			offset = _sizeFinal.width;
			step_coeef = -1;
		}
		else if (mFlowToDirection == Align::Top)
		{
			offset = _sizeFinal.height;
			step_coeef = -1;
		}

		EnumeratorWidgetPtr child = getEnumerator();
		while (child.next())
		{
			if (!child->isVisible())
				continue;

			const IntSize& child_size = child->getDesiredSize();
			Align align = child->getAlign();

			IntCoord coord;
			if (mFlowToDirection == Align::Bottom)
			{
				item_offset = mUniform ? mMaxItemSize.height : child_size.height;
				if (!align.isTop() && positiv_diff)
				{
					item_offset += positiv_diff;
					positiv_diff = 0;
				}
				coord.set(0, offset, _sizeFinal.width, item_offset);
			}
			else if (mFlowToDirection == Align::Top)
			{
				item_offset = mUniform ? mMaxItemSize.height : child_size.height;
				if (!align.isBottom() && positiv_diff)
				{
					item_offset += positiv_diff;
					positiv_diff = 0;
				}
				coord.set(0, offset - item_offset, _sizeFinal.width, item_offset);
			}
			else if (mFlowToDirection == Align::Left)
			{
				item_offset = mUniform ? mMaxItemSize.width : child_size.width;
				if (!align.isRight() && positiv_diff)
				{
					item_offset += positiv_diff;
					positiv_diff = 0;
				}
				coord.set(offset - item_offset, 0, item_offset, _sizeFinal.height);
			}
			else if (mFlowToDirection == Align::Right)
			{
				item_offset = mUniform ? mMaxItemSize.width : child_size.width;
				if (!align.isLeft() && positiv_diff)
				{
					item_offset += positiv_diff;
					positiv_diff = 0;
				}
				coord.set(offset, 0, item_offset, _sizeFinal.height);
			}

			if (align.isLeft())
				coord.width = child_size.width;
			else if (align.isRight())
			{
				coord.left = coord.width - child_size.width + coord.left;
				coord.width = child_size.width;
			}
			else if (align.isHCenter())
			{
				coord.left = (coord.width - child_size.width) / 2 + coord.left;
				coord.width = child_size.width;
			}

			if (align.isTop())
				coord.height = child_size.height;
			else if (align.isBottom())
			{
				coord.top = coord.height - child_size.height + coord.top;
				coord.height = child_size.height;
			}
			else if (align.isVCenter())
			{
				coord.top = ((coord.height - child_size.height) / 2) + coord.top;
				coord.height = child_size.height;
			}

			child->setArrange(coord);
			offset += (item_offset + mSpacer) * step_coeef;
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

	void StackPanel::setFlowToDirection(Align _value)
	{
		mFlowToDirection = _value;
		if (mFlowToDirection != Align::Left && mFlowToDirection != Align::Right && mFlowToDirection != Align::Top)
			mFlowToDirection = Align::Bottom;
		invalidateMeasure();
	}

} // namespace MyGUI
