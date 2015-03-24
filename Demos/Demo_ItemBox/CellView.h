/*!
	@file
	@author		Albert Semenov
	@date		07/2008
*/
#ifndef CELL_VIEW_H_
#define CELL_VIEW_H_

#include <MyGUI.h>
#include "ItemBox/BaseCellView.h"
#include "ItemData.h"

namespace demo
{

	class CellView :
		public wraps::BaseCellView<ItemData*>
	{
	public:
		CellView(MyGUI::Widget* _parent);

		void update(const MyGUI::IBDrawItemInfo& _info, ItemData* _data);
		static void getCellDimension(MyGUI::Widget* _sender, MyGUI::IntCoord& _coord, bool _drop);

	private:
		MyGUI::ImageBox* mImageBack;
		MyGUI::ImageBox* mImageBorder;
		MyGUI::ImageBox* mImageItem;
		MyGUI::TextBox* mTextBack;
		MyGUI::TextBox* mTextFront;
	};

} // namespace demo

#endif // CELL_VIEW_H_
