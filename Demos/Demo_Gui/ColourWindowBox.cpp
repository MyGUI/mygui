/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include "ColourWindowBox.h"

namespace demo
{

	ColourWindowBox::ColourWindowBox()
	{
	}

	void ColourWindowBox::initialise(MyGUI::WidgetPtr _parent)
	{
		wrap(_parent);
	}

	void ColourWindowBox::shutdown()
	{
		size_t count = getItemBox()->getItemCount();
		for (size_t pos=0; pos<count; ++pos) {
			delete *getItemBox()->getItemDataAt<ColourWindowCellData*>(pos);
		}

		wraps::BaseItemBox<ColourWindowCellView>::shutdown();
	}

	void ColourWindowBox::addColourItem(const MyGUI::Colour& _colour, const std::string& _name)
	{
		addItem(new ColourWindowCellData(_colour, _name));
	}

} // namespace demo
