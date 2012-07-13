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
#include "ActionSwapData.h"

namespace tools
{
	FACTORY_ITEM_ATTRIBUTE(ImageListControl);

	ImageListControl::ImageListControl() :
		mNameIndex(0),
		mListBox(nullptr),
		mLastIndex(MyGUI::ITEM_NONE)
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

		assignWidget(mListBox, "ListBoxControl");

		DataManager::getInstance().eventChangeData.connect(this, &ImageListControl::notifyChangeData);
		mListBox->eventListChangePosition += MyGUI::newDelegate(this, &ImageListControl::notifyListChangePosition);
	}

	bool ImageListControl::checkCommand()
	{
		if (DialogManager::getInstance().getAnyDialog())
			return false;

		if (MessageBoxManager::getInstance().hasAny())
			return false;

		return true;
	}

	void ImageListControl::commandCreateData(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		ActionCreateData* command = new ActionCreateData();
		command->setName(MyGUI::utility::toString("item ", mNameIndex));
		command->setParent(DataManager::getInstance().getRoot());

		ActionManager::getInstance().doAction(command);

		mNameIndex ++;

		_result = true;
	}

	void ImageListControl::commandDestroyData(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		size_t index = mListBox->getIndexSelected();
		if (index != MyGUI::ITEM_NONE)
		{
			Data* data = *mListBox->getItemDataAt<Data*>(index);

			ActionDestroyData* command = new ActionDestroyData();
			command->setData(data);

			ActionManager::getInstance().doAction(command);
		}

		_result = true;
	}

	void ImageListControl::commandRenameData(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		size_t index = mListBox->getIndexSelected();
		if (index != MyGUI::ITEM_NONE)
		{
			Data* data = *mListBox->getItemDataAt<Data*>(index);

			ActionRenameData* command = new ActionRenameData();
			command->setData(data);
			command->setName(MyGUI::utility::toString("item ", mNameIndex));

			ActionManager::getInstance().doAction(command);
		}

		mNameIndex ++;

		_result = true;
	}

	void ImageListControl::notifyChangeData()
	{
		mListBox->setIndexSelected(MyGUI::ITEM_NONE);
		mLastIndex = MyGUI::ITEM_NONE;

		for (size_t index = 0; index < mListBox->getItemCount(); index ++)
			mListBox->setItemDataAt(index, nullptr);

		const Data::VectorData& childs = DataManager::getInstance().getRoot()->getChilds();

		while (mListBox->getItemCount() > childs.size())
			mListBox->removeItemAt(mListBox->getItemCount() - 1);

		while (mListBox->getItemCount() < childs.size())
			mListBox->addItem("", nullptr);

		for (size_t index = 0; index < childs.size(); index ++)
		{
			mListBox->setItemNameAt(index, childs.at(index)->getPropertyValue("Name"));
			mListBox->setItemDataAt(index, childs.at(index));
		}
	}

	void ImageListControl::notifyListChangePosition(MyGUI::ListBox* _sender, size_t _index)
	{
		if (mLastIndex != MyGUI::ITEM_NONE && _index != MyGUI::ITEM_NONE)
		{
			if (MyGUI::InputManager::getInstance().isControlPressed())
			{
				Data* data1 = *mListBox->getItemDataAt<Data*>(mLastIndex);
				Data* data2 = *mListBox->getItemDataAt<Data*>(_index);

				ActionSwapData* command = new ActionSwapData();
				command->setData1(data1);
				command->setData2(data2);

				ActionManager::getInstance().doAction(command);
			}
		}

		mLastIndex = _index;
	}
}
