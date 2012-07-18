/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "AreaSelectorControl.h"

namespace tools
{

	AreaSelectorControl::AreaSelectorControl(MyGUI::Widget* _parent) :
		SelectorControl("AreaSelectorControl.layout", _parent)
	{
		setPropertyColour("ColourSelector");
	}

	AreaSelectorControl::~AreaSelectorControl()
	{
	}

}
