/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __TOOL_TIP_H__
#define __TOOL_TIP_H__

#include <MyGUI.h>
#include "BaseLayout.h"
#include "ItemData.h"

class ToolTipWindow : public BaseLayout
{
public:
	ToolTipWindow();

	virtual void initialise();

	void show(ItemData2 * _data, const MyGUI::IntPoint & _point);

private:

	MyGUI::StaticTextPtr mTextName;
	MyGUI::StaticTextPtr mTextCount;
	MyGUI::StaticTextPtr mTextDesc;
	MyGUI::StaticImagePtr mImageInfo;
};

#endif // __TOOL_TIP_H__
