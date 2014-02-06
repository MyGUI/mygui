/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "AreaSelectorControl.h"

namespace tools
{

	AreaSelectorControlLE::AreaSelectorControlLE(MyGUI::Widget* _parent) :
		SelectorControlLE("AreaSelectorControl.layout", _parent)
	{
		setPropertyColour("ColourSelector");
	}

	AreaSelectorControlLE::~AreaSelectorControlLE()
	{
	}

}
