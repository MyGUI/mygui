/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "StateListControl.h"
#include "FactoryManager.h"
#include "CommandManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(StateListControl)

	StateListControl::StateListControl()
	{
	}

	StateListControl::~StateListControl()
	{
	}

	void StateListControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		DataListBaseControl::OnInitialise(_parent, _place, _layoutName);

		setDataInfo("Skin", "State", "Name", "");
	}

}
