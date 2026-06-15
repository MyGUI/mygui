/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PositionSelectorBlackControl.h"

namespace tools
{

	void PositionSelectorBlackControl::OnInitialise(
		Control* _parent,
		MyGUI::Widget* _place,
		std::string_view /*_layoutName*/)
	{
		SelectorControl::OnInitialise(_parent, _place, "PositionSelectorControl.layout");
		setPropertyColour("ColourSelectorInactive");
		mMainWidget->setNeedMouseFocus(false);
	}

}
