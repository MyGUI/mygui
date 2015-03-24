/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PositionSelectorControl.h"

namespace tools
{

	PositionSelectorControlLE::PositionSelectorControlLE(MyGUI::Widget* _parent) :
		SelectorControlLE("PositionSelectorControl.layout", _parent)
	{
		setPropertyColour("ColourSelector");
	}

	PositionSelectorControlLE::~PositionSelectorControlLE()
	{
	}

}
