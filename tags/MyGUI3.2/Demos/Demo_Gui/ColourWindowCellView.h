/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef __COLOUR_WINDOW_CELL_VIEW_H__
#define __COLOUR_WINDOW_CELL_VIEW_H__

#include <MyGUI.h>
#include "ItemBox/BaseCellView.h"
#include "ColourWindowCellData.h"

namespace demo
{
	class ColourWindowCellView :
		public wraps::BaseCellView<ColourWindowCellData*>
	{
	public:
		ColourWindowCellView(MyGUI::Widget* _parent);

		void update(const MyGUI::IBDrawItemInfo& _info, ColourWindowCellData* _data);
		static void getCellDimension(MyGUI::Widget* _sender, MyGUI::IntCoord& _coord, bool _drop);

	private:
		MyGUI::TextBox* mText;
		MyGUI::Widget* mColour;
	};

} // namespace demo

#endif // __COLOUR_WINDOW_CELL_VIEW_H__
