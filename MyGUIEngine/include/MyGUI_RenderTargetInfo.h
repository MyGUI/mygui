/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_RENDER_TARGET_INFO_H__
#define __MYGUI_RENDER_TARGET_INFO_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	struct MYGUI_EXPORT RenderTargetInfo
	{
	public:
		RenderTargetInfo() :
			maximumDepth(0),
			pixScaleX(1),
			pixScaleY(1),
			hOffset(0),
			vOffset(0),
			aspectCoef(1),
			leftOffset(0),
			topOffset(0)
		{
		}

		void setOffset(int _left, int _top) const
		{
			leftOffset = _left;
			topOffset = _top;
		}

	public:
		float maximumDepth;
		float pixScaleX;
		float pixScaleY;
		float hOffset;
		float vOffset;
		float aspectCoef;

		mutable int leftOffset;
		mutable int topOffset;
	};


} // namespace MyGUI

#endif // __MYGUI_RENDER_TARGET_INFO_H__
