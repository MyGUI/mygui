/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
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
		if (_update)
			mTextOutDate = true;

		if (nullptr == mFont)
			return;
		if (!mVisible || mEmptyView)
			return;

		if (mTextOutDate)
			updateRawData();

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
