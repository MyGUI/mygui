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
		mNameIndex(0),
		mListBoxControl(nullptr)
	{
	}

	ImageListControl::~ImageListControl()
	{
	}

	void ImageListControl::Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::Initialise(_parent, _place, _layoutName);

		CommandManager::getInstance().registerCommand("Command_CreateImageData", MyGUI::newDelegate(this, &ImageListControl::commandCreateImageData));
		CommandManager::getInstance().registerCommand("Command_DestroyImageData", MyGUI::newDelegate(this, &ImageListControl::commandDestroyImageData));
		CommandManager::getInstance().registerCommand("Command_RenameImageData", MyGUI::newDelegate(this, &ImageListControl::commandRenameImageData));

		const VectorControl& childs = getChilds();
		for (VectorControl::const_iterator child = childs.begin(); child != childs.end(); child ++)
		{
			ListBoxDataControl* list = dynamic_cast<ListBoxDataControl*>(*child);
			if (list != nullptr)
			{
				mListBoxControl = list;
				break;
			}
		}

		if (mListBoxControl != nullptr)
		{
			mListBoxControl->setEnableChangePosition(true);
		}
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

	void ImageListControl::commandCreateImageData(const MyGUI::UString& _commandName, bool& _result)
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

	void ImageListControl::commandDestroyImageData(const MyGUI::UString& _commandName, bool& _result)
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

	void ImageListControl::commandRenameImageData(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand(_result))
			return;

		if (mListBoxControl != nullptr)
			mListBoxControl->OnRenameData();

		_result = true;
	}

	void ImageListControl::OnCommand(const std::string& _commandName, MyGUI::Any _data)
	{
		if (_commandName == "OnChangePositionData")
		{
			typedef std::pair<Data*, Data*> PairData;
			PairData *pairData = _data.castType<PairData>(false);
			if (pairData != nullptr)
			{
				ActionChangePositionData* command = new ActionChangePositionData();
				command->setData1(pairData->first);
				command->setData2(pairData->second);

				ActionManager::getInstance().doAction(command);
			}
		}
		else if (_commandName == "OnChangeNameData")
		{
			typedef std::pair<Data*, MyGUI::UString> PairData;
			PairData *pairData = _data.castType<PairData>(false);
			if (pairData != nullptr)
			{
				ActionRenameData* command = new ActionRenameData();
				command->setData(pairData->first);
				command->setName(pairData->second);

				ActionManager::getInstance().doAction(command);
			}
		}
		else
		{
			Control::OnCommand(_commandName, _data);
		}
	}
}
