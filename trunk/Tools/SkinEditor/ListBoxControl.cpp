/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#include "Precompiled.h"
#include "ListBoxControl.h"

namespace tools
{
	ListBoxControl::ListBoxControl(MyGUI::Widget* _parent) :
		wraps::BaseItemBox<ListBoxItemControl>(_parent)
	{
	}

	ListBoxControl::~ListBoxControl()
	{
		MyGUI::ItemBox* box = getItemBox();
		size_t count = box->getItemCount();
		for (size_t pos = 0; pos < count; ++pos)
		{
			delete *box->getItemDataAt<ListBoxItemData*>(pos);
		}
	}
}
