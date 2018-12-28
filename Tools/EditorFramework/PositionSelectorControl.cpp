/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PositionSelectorControl.h"

namespace tools
{

	void PositionSelectorControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		SelectorControl::OnInitialise(_parent, _place, "PositionSelectorControl.layout");
		setPropertyColour("ColourSelector");
	}

}
