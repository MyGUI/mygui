/*!
	@file
	@author		Albert Semenov
	@date		07/2008
*/
#include "Precompiled.h"
#include "ItemBoxWindow.h"

namespace demo
{

	ItemBoxWindow::ItemBoxWindow(std::string_view _layout) :
		BaseLayout(_layout)
	{
		assignBase(mItemBox, "box_Items");
	}

} // namespace demo
