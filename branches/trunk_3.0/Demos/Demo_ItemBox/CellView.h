/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __CELL_VIEW_H__
#define __CELL_VIEW_H__

#include <MyGUI.h>
#include "ItemBox/BaseCellView.h"
#include "ItemData.h"

namespace demo
{

	class CellView : public wraps::BaseCellView<ItemData>
	{
	public:
		CellView(MyGUI::Widget* _parent);

		void update(const MyGUI::IBDrawItemInfo & _info, ItemData * _data);
		static void getCellDimension(MyGUI::Widget* _sender, MyGUI::IntCoord & _coord, bool _drop);

	private:
		MyGUI::StaticImage* mImageBack;
		MyGUI::StaticImage* mImageBorder;
		MyGUI::StaticImage* mImageItem;
		MyGUI::StaticText* mTextBack;
		MyGUI::StaticText* mTextFront;

	};

} // namespace demo

#endif // __CELL_VIEW_H__
