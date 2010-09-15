/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "VerticalSelectorBlackControl.h"

namespace tools
{

	VerticalSelectorBlackControl::VerticalSelectorBlackControl(MyGUI::Widget* _parent) :
		SelectorControl("VerticalSelectorControl.layout", _parent)
	{
		setPropertyColour("ColourSelectorInactive");
	}

	VerticalSelectorBlackControl::~VerticalSelectorBlackControl()
	{
	}

} // namespace tools
