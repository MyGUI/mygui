/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "PositionSelectorBlackControl.h"

namespace tools
{

	PositionSelectorBlackControl::PositionSelectorBlackControl(MyGUI::Widget* _parent) :
		SelectorControl("PositionSelectorControl.layout", _parent)
	{
		setPropertyColour("ColourSelectorInactive");
		mMainWidget->setNeedMouseFocus(false);
	}

	PositionSelectorBlackControl::~PositionSelectorBlackControl()
	{
	}

} // namespace tools
