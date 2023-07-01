/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef COLOUR_WINDOW_BOX_H_
#define COLOUR_WINDOW_BOX_H_

#include <MyGUI.h>
#include "ItemBox/BaseItemBox.h"
#include "ColourWindowCellView.h"

namespace demo
{
	class ColourWindowBox : public wraps::BaseItemBox<ColourWindowCellView>
	{
	public:
		ColourWindowBox(MyGUI::Widget* _parent);
		~ColourWindowBox() override;

		void addColourItem(const MyGUI::Colour& _colour, std::string_view _name);
	};

} // namespace demo

#endif // COLOUR_WINDOW_BOX_H_
