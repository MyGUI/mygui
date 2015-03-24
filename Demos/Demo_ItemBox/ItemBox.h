/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef ITEM_BOX_H_
#define ITEM_BOX_H_

#include <MyGUI.h>
#include "ItemBox/BaseItemBox.h"
#include "CellView.h"

namespace demo
{

	class ItemBox :
		public wraps::BaseItemBox<CellView>
	{
	public:
		ItemBox(MyGUI::Widget* _parent);
		virtual ~ItemBox();
	};

} // namespace demo

#endif // ITEM_BOXV_H_
