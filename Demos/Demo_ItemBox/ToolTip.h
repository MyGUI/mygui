/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __TOOL_TIP_H__
#define __TOOL_TIP_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"
#include "ItemData.h"

namespace demo
{

	class ToolTip : public wraps::BaseLayout
	{
	public:
		ToolTip();

		void show(ItemData * _data, const MyGUI::IntPoint & _point);
		void hide();

	private:

		MyGUI::StaticTextPtr mTextName;
		MyGUI::StaticTextPtr mTextCount;
		MyGUI::EditPtr mTextDesc;
		MyGUI::StaticImagePtr mImageInfo;

		int mOffsetHeight;
	};

} // namespace demo

#endif // __TOOL_TIP_H__
