/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#include "precompiled.h"
#include "ItemBox.h"

namespace demo
{

	ItemBox::ItemBox(MyGUI::WidgetPtr _parent) : wraps::BaseItemBox<CellView>("", _parent)
	{
	}

	ItemBox::~ItemBox()
	{
		MyGUI::ItemBoxPtr box = getItemBox();
		size_t count = box->getItemCount();
		for (size_t pos=0; pos<count; ++pos) {
			delete *box->getItemDataAt<ItemData*>(pos);
		}
	}

} // namespace demo
