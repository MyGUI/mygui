/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "Precompiled.h"
#include "ControlDebug.h"
#include "FactoryManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(ControlDebug)

	ControlDebug::ControlDebug()
	{
	}

	ControlDebug::~ControlDebug()
	{
	}

	void ControlDebug::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);
	}

}
