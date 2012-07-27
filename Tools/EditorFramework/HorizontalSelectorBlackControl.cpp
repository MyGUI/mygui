/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "HorizontalSelectorBlackControl.h"

namespace tools
{

	HorizontalSelectorBlackControl::HorizontalSelectorBlackControl()
	{
	}

	HorizontalSelectorBlackControl::~HorizontalSelectorBlackControl()
	{
	}

	void HorizontalSelectorBlackControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		SelectorControl::OnInitialise(_parent, _place, "HorizontalSelectorControl.layout");
		setPropertyColour("ColourSelectorInactive");
		mMainWidget->setNeedMouseFocus(false);
	}

}
