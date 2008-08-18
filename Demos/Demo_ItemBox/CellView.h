/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __CELL_VIEW_H__
#define __CELL_VIEW_H__

#include <MyGUI.h>
#include "BaseCellView.h"
#include "ItemData.h"

template class BaseCellView<ItemData>;

class CellView : public BaseCellView<ItemData>
{
public:
	CellView();

	void initialise(MyGUI::WidgetPtr _parent);

	//void getSizeCell(WidgetPtr _client, MyGUI::IntCoord & _coord, bool _drop);
	void update(const MyGUI::ItemInfo& _info, ItemData * _data);

private:

	MyGUI::StaticImagePtr mImageBack;
	MyGUI::StaticImagePtr mImageBorder;
	MyGUI::StaticImagePtr mImageItem;
	MyGUI::StaticTextPtr mTextBack;
	MyGUI::StaticTextPtr mTextFront;

};

#endif // __CELL_VIEW_H__
