/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "VerticalSelectorControl.h"

namespace tools
{

	VerticalSelectorControl::VerticalSelectorControl()
	{
	}

	VerticalSelectorControl::~VerticalSelectorControl()
	{
	}

	void VerticalSelectorControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		SelectorControl::OnInitialise(_parent, _place, "VerticalSelectorControl.layout");
		setPropertyColour("ColourSelector");
	}

}
