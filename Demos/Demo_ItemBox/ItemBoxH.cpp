/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#include "ItemBoxH.h"

namespace demo
{
	
	ItemBoxH::ItemBoxH() :
		BaseLayout("ItemBoxH.layout")
	{
	}

	void ItemBoxH::initialise()
	{
		BaseLayout::initialise();
		wrapWidget(mItemBox, "box_Items");
	}

	void ItemBoxH::shutdown()
	{
		mItemBox.shutdown();
		BaseLayout::shutdown();
	}

} // namespace demo
