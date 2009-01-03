/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __ITEM_BOXH_H__
#define __ITEM_BOXH_H__

#include <MyGUI.h>
#include "ItemBox/BaseItemBox.h"
#include "CellView.h"

namespace demo
{

	class ItemBoxH : public wraps::BaseLayout
	{
	public:
		ItemBoxH();

		virtual void initialise();
		virtual void shutdown();
		wraps::BaseItemBox<CellView> & getItemBox() { return mItemBox; }

	private:
		wraps::BaseItemBox<CellView> mItemBox;

	};

} // namespace demo

#endif // __ITEM_BOXH_H__
