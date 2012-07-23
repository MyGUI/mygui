/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "HorizontalSelectorControl.h"

namespace tools
{

	HorizontalSelectorControl::HorizontalSelectorControl()
	{
	}

	HorizontalSelectorControl::~HorizontalSelectorControl()
	{
	}

	void HorizontalSelectorControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		SelectorControl::OnInitialise(_parent, _place, "HorizontalSelectorControl.layout");
		setPropertyColour("ColourSelector");
	}

}
