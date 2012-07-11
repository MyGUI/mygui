/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "VerticalSelectorControl.h"

namespace tools
{

	VerticalSelectorControl::VerticalSelectorControl(MyGUI::Widget* _parent) :
		SelectorControl("VerticalSelectorControl.layout", _parent)
	{
		setPropertyColour("ColourSelector");
	}

	VerticalSelectorControl::~VerticalSelectorControl()
	{
	}

} // namespace tools
