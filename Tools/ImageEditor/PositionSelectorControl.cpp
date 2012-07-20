/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PositionSelectorControl.h"

namespace tools
{

	PositionSelectorControl::PositionSelectorControl(MyGUI::Widget* _parent) :
		SelectorControl("PositionSelectorControl.layout", _parent)
	{
		setPropertyColour("ColourSelector");
	}

	PositionSelectorControl::~PositionSelectorControl()
	{
	}

}
