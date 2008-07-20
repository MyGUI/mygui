/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __CELL_VIEW_H__
#define __CELL_VIEW_H__

#include <MyGUI.h>
#include "BaseLayout.h"
#include "ItemData.h"

class CellView : public BaseLayout
{
public:
	CellView();

	void initialise(MyGUI::WidgetPtr _parent);

	void update(const MyGUI::ItemInfo& _info, ItemData * _data);

private:

	MyGUI::StaticImagePtr mImageBack;
	MyGUI::StaticImagePtr mImageBorder;
	MyGUI::StaticImagePtr mImageItem;
	MyGUI::StaticTextPtr mTextBack;
	MyGUI::StaticTextPtr mTextFront;

};

#endif // __CELL_VIEW_H__
