/*!
	@file
	@author		Albert Semenov
	@date		07/2008
*/
#ifndef __ITEM_BOXV_H__
#define __ITEM_BOXV_H__

#include <MyGUI.h>
#include "ItemBox.h"
#include "CellView.h"

namespace demo
{

	class ItemBoxWindow :
		public wraps::BaseLayout
	{
	public:
		ItemBoxWindow(const std::string& _layout);
		ItemBox* getItemBox()
		{
			return mItemBox;
		}

	private:
		ItemBox* mItemBox;
	};

} // namespace demo

#endif // __ITEM_BOXV_H__
