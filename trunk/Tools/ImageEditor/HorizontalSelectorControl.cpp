/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "HorizontalSelectorControl.h"

namespace tools
{

	HorizontalSelectorControl::HorizontalSelectorControl(MyGUI::Widget* _parent) :
		SelectorControl("HorizontalSelectorControl.layout", _parent)
	{
		setPropertyColour("ColourSelector");
	}

	HorizontalSelectorControl::~HorizontalSelectorControl()
	{
	}

} // namespace tools
