/*!
	@file
	@author		Albert Semenov
	@date		07/2008
*/
#ifndef __TOOL_TIP_H__
#define __TOOL_TIP_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"
#include "ItemData.h"

namespace demo
{

	class ToolTip :
		public wraps::BaseLayout
	{
	public:
		ToolTip();

		void show(ItemData* _data);
		void hide();
		void move(const MyGUI::IntPoint& _point);

	private:
		MyGUI::TextBox* mTextName;
		MyGUI::TextBox* mTextCount;
		MyGUI::EditBox* mTextDesc;
		MyGUI::ImageBox* mImageInfo;

		int mOffsetHeight;
	};

} // namespace demo

#endif // __TOOL_TIP_H__
