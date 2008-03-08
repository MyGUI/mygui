/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_ONE_OVERLAY_DATA_INFO_H__
#define __MYGUI_ONE_OVERLAY_DATA_INFO_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	struct OneOverlayDataInfo
	{
		OneOverlayDataInfo() : offsetX(0), offsetY(0), left(0), top(0), width(0), height(0), u1(0), v1(0), u2(0), v2(0), transparent(false) {}

		float offsetX, offsetY;
		float left, top, width, height;
		float u1, v1, u2, v2;
		bool transparent;
	};
	typedef std::vector<OneOverlayDataInfo> VectorSharedOverlay;

} // namespace MyGUI

#endif // __MYGUI_ONE_OVERLAY_DATA_INFO_H__
