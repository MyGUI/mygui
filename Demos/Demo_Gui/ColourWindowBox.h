/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __COLOUR_WINDOW_BOX_H__
#define __COLOUR_WINDOW_BOX_H__

#include <MyGUI.h>
#include "ItemBox\BaseItemBox.h"
#include "ColourWindowCellView.h"

namespace demo
{

	class ColourWindowBox : public wraps::BaseItemBox<ColourWindowCellView>
	{
	public:
		ColourWindowBox();

		void initialise(MyGUI::WidgetPtr _parent);

		void addColourItem(const MyGUI::Colour& _colour, const std::string& _name);

	};

} // namespace demo

#endif // __COLOUR_WINDOW_BOX_H__
