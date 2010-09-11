/*!
	@file
	@author		Albert Semenov
	@date		07/2008
*/
#include "precompiled.h"
#include "ItemBox.h"

namespace demo
{

	ItemBox::ItemBox(MyGUI::Widget* _parent) :
		wraps::BaseItemBox<TextureBrowseCell>("", _parent)
	{
	}

	ItemBox::~ItemBox()
	{
	}

} // namespace demo
