/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef COLOUR_WINDOW_CELL_VIEW_H_
#define COLOUR_WINDOW_CELL_VIEW_H_

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

#endif // COLOUR_WINDOW_CELL_VIEW_H_
