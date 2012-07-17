/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "GroupListControl.h"
#include "FactoryManager.h"
#include "CommandManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(GroupListControl)

	GroupListControl::GroupListControl()
	{
	}

	GroupListControl::~GroupListControl()
	{
	}

	void GroupListControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		DataListBaseControl::OnInitialise(_parent, _place, _layoutName);

		CommandManager::getInstance().registerCommand("Command_CreateGroupData", MyGUI::newDelegate((DataListBaseControl*)this, &DataListBaseControl::commandCreateImageData));
		CommandManager::getInstance().registerCommand("Command_DestroyGroupData", MyGUI::newDelegate((DataListBaseControl*)this, &DataListBaseControl::commandDestroyImageData));
		CommandManager::getInstance().registerCommand("Command_RenameGroupData", MyGUI::newDelegate((DataListBaseControl*)this, &DataListBaseControl::commandRenameImageData));

		setDataInfo("Image", "Group", "Name", "UniqueName");
	}

}
