/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_RENDER_TARGET_INFO_H_
#define MYGUI_RENDER_TARGET_INFO_H_

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	struct MYGUI_EXPORT RenderTargetInfo
	{
	public:
		void setOffset(int _left, int _top) const
		{
			leftOffset = _left;
			topOffset = _top;
		}

	public:
		float maximumDepth{0};
		float pixScaleX{1};
		float pixScaleY{1};
		float hOffset{0};
		float vOffset{0};
		float aspectCoef{1};

		mutable int leftOffset{0};
		mutable int topOffset{0};
	};


} // namespace MyGUI

#endif // MYGUI_RENDER_TARGET_INFO_H_
