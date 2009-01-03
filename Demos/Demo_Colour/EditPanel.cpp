/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include "EditPanel.h"

namespace demo
{

	EditPanel::EditPanel() :
		BaseLayout("EditPanel.layout")
	{
	}

	void EditPanel::initialise()
	{
		loadLayout();

		assignWidget(mEdit, "Edit");
	}

	void EditPanel::shutdown()
	{
		BaseLayout::shutdown();
	}

	void EditPanel::setColourSelected(const Ogre::ColourValue& _colour)
	{
		mEdit->setTextSelectColour(_colour);
	}

} // namespace demo
