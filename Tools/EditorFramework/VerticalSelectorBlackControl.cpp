/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "VerticalSelectorBlackControl.h"

namespace tools
{

	void VerticalSelectorBlackControl::OnInitialise(
		Control* _parent,
		MyGUI::Widget* _place,
		std::string_view /*_layoutName*/)
	{
		SelectorControl::OnInitialise(_parent, _place, "VerticalSelectorControl.layout");
		setPropertyColour("ColourSelectorInactive");
		mMainWidget->setNeedMouseFocus(false);
	}

}
