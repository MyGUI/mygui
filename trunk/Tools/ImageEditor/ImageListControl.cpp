/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#include "Precompiled.h"
#include "ImageListControl.h"
#include "FactoryItemAttribute.h"
#include "FactoryManager.h"
#include "CommandManager.h"
#include "Tools/DialogManager.h"
#include "MessageBoxManager.h"
#include "DataManager.h"
#include "ActionManager.h"
#include "ActionCreateData.h"
#include "ActionDestroyData.h"
#include "ActionRenameData.h"
#include "ActionChangePositionData.h"

namespace tools
{
	FACTORY_ITEM_ATTRIBUTE(ImageListControl);

	ImageListControl::ImageListControl() :
		mNameIndex(0)
	{
	}

	ImageListControl::~ImageListControl()
	{
	}

	void ImageListControl::Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::Initialise(_parent, _place, _layoutName);

		CommandManager::getInstance().registerCommand("Command_CreateData", MyGUI::newDelegate(this, &ImageListControl::commandCreateData));
		CommandManager::getInstance().registerCommand("Command_DestroyData", MyGUI::newDelegate(this, &ImageListControl::commandDestroyData));
		CommandManager::getInstance().registerCommand("Command_RenameData", MyGUI::newDelegate(this, &ImageListControl::commandRenameData));
	}

	bool ImageListControl::checkCommand(bool _result)
	{
		if (_result)
			return false;

		if (DialogManager::getInstance().getAnyDialog())
			return false;

		if (MessageBoxManager::getInstance().hasAny())
			return false;

		return true;
	}

	void ImageListControl::commandCreateData(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand(_result))
			return;

		Data* data = DataManager::getInstance().getSelectedDataByType("Root");
		ActionCreateData* command = new ActionCreateData();
		command->setName(MyGUI::utility::toString("item ", mNameIndex));
		command->setParent(data);

		ActionManager::getInstance().doAction(command);

		mNameIndex ++;

		_result = true;
	}

	void ImageListControl::commandDestroyData(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand(_result))
			return;

		Data* data = DataManager::getInstance().getSelectedDataByType("Image");
		if (data != nullptr)
		{
			ActionDestroyData* command = new ActionDestroyData();
			command->setData(data);

			ActionManager::getInstance().doAction(command);
		}

		_result = true;
	}

	void ImageListControl::commandRenameData(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand(_result))
			return;

		Data* data = DataManager::getInstance().getSelectedDataByType("Image");
		if (data != nullptr)
		{
			ActionRenameData* command = new ActionRenameData();
			command->setData(data);
			command->setName(MyGUI::utility::toString("item ", mNameIndex));

			ActionManager::getInstance().doAction(command);
		}

		mNameIndex ++;

		_result = true;
	}

	void ImageListControl::OnCommand(const std::string& _commandName, MyGUI::Any _data)
	{
		if (_commandName == "OnChangePositionData")
		{
			typedef std::pair<Data*, Data*> PairData;
			PairData *data = _data.castType<PairData>(false);
			if (data != nullptr)
			{
				ActionSwapData* command = new ActionSwapData();
				command->setData1(data->first);
				command->setData2(data->second);

				ActionManager::getInstance().doAction(command);
			}
		}
		else
		{
			Control::OnCommand(_commandName, _data);
		}
	}
}
