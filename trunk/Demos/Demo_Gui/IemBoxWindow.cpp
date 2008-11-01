/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/

#include "ItemBoxWindow.h"

namespace demo
{

	ItemBoxWindow::ItemBoxWindow() :
		BaseLayout("ItemBoxWindow.layout")
	{
	}

	void ItemBoxWindow::initialise()
	{
		loadLayout();

	}

} // namespace demo
