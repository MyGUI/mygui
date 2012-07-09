/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef __LIST_BOX_ITEM_CONTROL_H__
#define __LIST_BOX_ITEM_CONTROL_H__

#include <MyGUI.h>
#include "ItemBox/BaseCellView.h"
#include "ListBoxItemData.h"

namespace tools
{
	class ListBoxItemControl :
		public wraps::BaseCellView<ListBoxItemData*>
	{
	public:
		ListBoxItemControl(MyGUI::Widget* _parent);

		void update(const MyGUI::IBDrawItemInfo& _info, ListBoxItemData* _data);
		static void getCellDimension(MyGUI::Widget* _sender, MyGUI::IntCoord& _coord, bool _drop);

	private:
		MyGUI::ImageBox* mImageBack;
		MyGUI::ImageBox* mImageBorder;
		MyGUI::ImageBox* mImageItem;
		MyGUI::TextBox* mTextBack;
		MyGUI::TextBox* mTextFront;
	};
}

#endif
