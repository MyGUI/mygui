/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "SkinListControl.h"
#include "FactoryManager.h"
#include "CommandManager.h"
#include "CreateSkinDataAction.h"
#include "ActionManager.h"
#include "DataUtility.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(SkinListControl)

	SkinListControl::SkinListControl()
	{
	}

	SkinListControl::~SkinListControl()
	{
	}

	void SkinListControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		DataListBaseControl::OnInitialise(_parent, _place, _layoutName);

		CommandManager::getInstance().getEvent("Command_CreateSkinData")->connect(this, &SkinListControl::commandCreateImageData);
		CommandManager::getInstance().getEvent("Command_CloneSkinData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandCloneData);
		CommandManager::getInstance().getEvent("Command_DestroySkinData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandDestroyData);
		CommandManager::getInstance().getEvent("Command_RenameSkinData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandRenameData);

		setDataInfo("Root", "Skin", "Name", "UniqueName");
	}

	void SkinListControl::commandCreateImageData(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand(_result))
			return;

		DataPtr data = DataUtility::getSelectedDataByType(getParentType());
		if (data != nullptr)
		{
			CreateSkinDataAction* command = new CreateSkinDataAction();
			command->setType(getCurrentType());
			command->setParent(data);
			command->setUniqueProperty(getPropertyForUnique());

			ActionManager::getInstance().doAction(command);
		}

		_result = true;
	}

}
