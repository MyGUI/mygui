/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#include "Precompiled.h"
#include "ListBoxItemControl.h"

namespace tools
{
	void ListBoxItemControl::getCellDimension(MyGUI::Widget* _sender, MyGUI::IntCoord& _coord, bool _drop)
	{
		_coord.set(0, 0, 90, 74);
	}

	ListBoxItemControl::ListBoxItemControl(MyGUI::Widget* _parent) :
		wraps::BaseCellView<ListBoxItemData*>("ListBoxItemControl.layout", _parent)
	{
		assignWidget(mImageBack, "image_Back");
		assignWidget(mImageBorder, "image_Border");
		assignWidget(mImageItem, "image_Item");
		assignWidget(mTextBack, "text_Back");
		assignWidget(mTextFront, "text_Front");
	}

	void ListBoxItemControl::update(const MyGUI::IBDrawItemInfo& _info, ListBoxItemData* _data)
	{
	}
}
