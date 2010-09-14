/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#include "Precompiled.h"
#include "ColourWindowBox.h"

namespace demo
{

	ColourWindowBox::ColourWindowBox(MyGUI::Widget* _parent) :
		wraps::BaseItemBox<ColourWindowCellView>(_parent)
	{
	}

	ColourWindowBox::~ColourWindowBox()
	{
		size_t count = getItemBox()->getItemCount();
		for (size_t pos = 0; pos < count; ++pos)
		{
			delete *getItemBox()->getItemDataAt<ColourWindowCellData*>(pos);
		}
	}

	void ColourWindowBox::addColourItem(const MyGUI::Colour& _colour, const std::string& _name)
	{
		addItem(new ColourWindowCellData(_colour, _name));
	}

} // namespace demo
