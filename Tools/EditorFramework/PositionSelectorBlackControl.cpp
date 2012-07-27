/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PositionSelectorBlackControl.h"

namespace tools
{

	PositionSelectorBlackControl::PositionSelectorBlackControl()
	{
	}

	PositionSelectorBlackControl::~PositionSelectorBlackControl()
	{
	}

	void PositionSelectorBlackControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		SelectorControl::OnInitialise(_parent, _place, "PositionSelectorControl.layout");
		setPropertyColour("ColourSelectorInactive");
		mMainWidget->setNeedMouseFocus(false);
	}

}
