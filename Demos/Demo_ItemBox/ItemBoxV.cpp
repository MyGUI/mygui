/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#include "ItemBoxV.h"

namespace demo
{

	ItemBoxV::ItemBoxV() :
		BaseLayout("ItemBoxV.layout")
	{
	}

	void ItemBoxV::initialise()
	{
		BaseLayout::initialise();
		wrapWidget(mItemBox, "box_Items");
	}

	void ItemBoxV::shutdown()
	{
		mItemBox.shutdown();
		BaseLayout::shutdown();
	}

} // namespace demo
