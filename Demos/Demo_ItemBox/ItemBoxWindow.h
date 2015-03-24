/*!
	@file
	@author		Albert Semenov
	@date		07/2008
*/
#ifndef ITEM_BOXV_H_
#define ITEM_BOXV_H_

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

#endif // ITEM_BOXV_H_
