/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __ITEM_BOX_H__
#define __ITEM_BOX_H__

#include <MyGUI.h>
#include "ItemBox/BaseItemBox.h"
#include "CellView.h"

namespace demo
{

	class ItemBox : public wraps::BaseItemBox<CellView>
	{
	public:
		ItemBox(MyGUI::WidgetPtr _parent);
		virtual ~ItemBox();

	};

} // namespace demo

#endif // __ITEM_BOXV_H__
