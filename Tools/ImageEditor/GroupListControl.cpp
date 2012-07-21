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

		CommandManager::getInstance().getEvent("Command_CreateGroupData")->connect((DataListBaseControl*)this, &DataListBaseControl::commandCreateImageData);
		CommandManager::getInstance().getEvent("Command_CloneGroupData")->connect((DataListBaseControl*)this, &DataListBaseControl::commandCloneImageData);
		CommandManager::getInstance().getEvent("Command_DestroyGroupData")->connect((DataListBaseControl*)this, &DataListBaseControl::commandDestroyImageData);
		CommandManager::getInstance().getEvent("Command_RenameGroupData")->connect((DataListBaseControl*)this, &DataListBaseControl::commandRenameImageData);

		setDataInfo("Image", "Group", "Name", "UniqueName");
	}

}
