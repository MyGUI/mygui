/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#include "precompiled.h"
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
	}

} // namespace demo
