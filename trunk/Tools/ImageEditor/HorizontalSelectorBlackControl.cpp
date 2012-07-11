/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "HorizontalSelectorBlackControl.h"

namespace tools
{

	HorizontalSelectorBlackControl::HorizontalSelectorBlackControl(MyGUI::Widget* _parent) :
		SelectorControl("HorizontalSelectorControl.layout", _parent)
	{
		setPropertyColour("ColourSelectorInactive");
		mMainWidget->setNeedMouseFocus(false);
	}

	HorizontalSelectorBlackControl::~HorizontalSelectorBlackControl()
	{
	}

} // namespace tools
