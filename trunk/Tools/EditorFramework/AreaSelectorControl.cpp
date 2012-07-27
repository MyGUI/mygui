/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "AreaSelectorControl.h"

namespace tools
{

	AreaSelectorControl::AreaSelectorControl()
	{
	}

	AreaSelectorControl::~AreaSelectorControl()
	{
	}

	void AreaSelectorControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		SelectorControl::OnInitialise(_parent, _place, "AreaSelectorControl.layout");
		setPropertyColour("ColourSelector");
	}

}
