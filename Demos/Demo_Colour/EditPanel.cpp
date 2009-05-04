/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#include "precompiled.h"
#include "EditPanel.h"

namespace demo
{

	EditPanel::EditPanel() : BaseLayout("EditPanel.layout")
	{
		assignWidget(mEdit, "Edit");
	}

	void EditPanel::setColourSelected(const MyGUI::Colour& _colour)
	{
		mEdit->setTextSelectionColour(_colour);
	}

} // namespace demo
