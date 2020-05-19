/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#include "Precompiled.h"
#include "EditPanel.h"

namespace demo
{

	EditPanel::EditPanel()
	{
		initialiseByAttributes(this);
	}

	void EditPanel::setColourSelected(const MyGUI::Colour& _colour)
	{
		mEdit->setTextSelectionColour(_colour);
		mEdit->setFontHeight(mEdit->getFontHeight() * 1.1);
	}

} // namespace demo
