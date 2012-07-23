/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "IndexListControl.h"
#include "FactoryManager.h"
#include "CommandManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(IndexListControl)

	IndexListControl::IndexListControl()
	{
	}

	IndexListControl::~IndexListControl()
	{
	}

	void IndexListControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		DataListBaseControl::OnInitialise(_parent, _place, _layoutName);

		CommandManager::getInstance().getEvent("Command_CreateIndexData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandCreateImageData);
		CommandManager::getInstance().getEvent("Command_CloneIndexData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandCloneImageData);
		CommandManager::getInstance().getEvent("Command_DestroyIndexData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandDestroyImageData);
		CommandManager::getInstance().getEvent("Command_RenameIndexData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandRenameImageData);

		setDataInfo("Group", "Index", "Name", "UniqueName");
	}

}
