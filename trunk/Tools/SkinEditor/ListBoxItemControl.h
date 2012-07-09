/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef __LIST_BOX_ITEM_CONTROL_H__
#define __LIST_BOX_ITEM_CONTROL_H__

#include <MyGUI.h>
#include "ItemBox/BaseCellView.h"
#include "SkinItem.h"

namespace tools
{
	class ListBoxItemControl :
		public wraps::BaseCellView<SkinItem*>
	{
	public:
		ListBoxItemControl(MyGUI::Widget* _parent);

		void update(const MyGUI::IBDrawItemInfo& _info, SkinItem* _data);
		static void getCellDimension(MyGUI::Widget* _sender, MyGUI::IntCoord& _coord, bool _drop);
	};
}

#endif
