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

	FACTORY_ITEM_ATTRIBUTE(IndexListControl);

	IndexListControl::IndexListControl()
	{
	}

	IndexListControl::~IndexListControl()
	{
	}

	void IndexListControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		DataListBaseControl::OnInitialise(_parent, _place, _layoutName);

		CommandManager::getInstance().registerCommand("Command_CreateIndexData", MyGUI::newDelegate((DataListBaseControl*)this, &DataListBaseControl::commandCreateImageData));
		CommandManager::getInstance().registerCommand("Command_DestroyIndexData", MyGUI::newDelegate((DataListBaseControl*)this, &DataListBaseControl::commandDestroyImageData));
		CommandManager::getInstance().registerCommand("Command_RenameIndexData", MyGUI::newDelegate((DataListBaseControl*)this, &DataListBaseControl::commandRenameImageData));

		setDataInfo("Group", "Index", "Name");
	}

}
