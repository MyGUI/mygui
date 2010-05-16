/*!
	@file
	@author		Albert Semenov
	@date		02/2008
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
#include "MyGUI_SimpleText.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_CommonStateInfo.h"
#include "MyGUI_RenderManager.h"

namespace MyGUI
{

	SimpleText::SimpleText() :
		EditText()
	{
		mIsAddCursorWidth = false;
	}

	SimpleText::~SimpleText()
	{
	}

	void SimpleText::setViewOffset(const IntPoint& _point)
	{
	}

	void SimpleText::doRender()
	{
		bool _update = mRenderItem->getCurrentUpdate();
		if (_update) mTextOutDate = true;

		if (nullptr == mFont) return;
		if (!mVisible || mEmptyView) return;

		if (mTextOutDate) updateRawData();

		const IntSize& size = mTextView.getViewSize();

		if (mTextAlign.isRight())
			mViewOffset.left = - (mCoord.width - size.width);
		else if (mTextAlign.isHCenter())
			mViewOffset.left = - ((mCoord.width - size.width) / 2);
		else
			mViewOffset.left = 0;

		if (mTextAlign.isBottom())
			mViewOffset.top = - (mCoord.height - size.height);
		else if (mTextAlign.isVCenter())
			mViewOffset.top = - ((mCoord.height - size.height) / 2);
		else
			mViewOffset.top = 0;

		Base::doRender();
	}

} // namespace MyGUI
