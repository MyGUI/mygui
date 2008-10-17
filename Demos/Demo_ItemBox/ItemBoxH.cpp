/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#include "ItemBoxH.h"

ItemBoxH::ItemBoxH() :
	BaseLayout("ItemBoxH.layout")
{
}

void ItemBoxH::initialise()
{
	BaseLayout::initialise();
	wrapWidget(mItemBox, "box_Items");
}
