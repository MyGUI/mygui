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
#include "DataSelectorManager.h"

namespace tools
{
	FACTORY_ITEM_ATTRIBUTE(ImageListControl);

	ImageListControl::ImageListControl() :
		mNameIndex(0),
		mListBox(nullptr),
		mParentData(nullptr)
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

		mListBox->eventListChangePosition += MyGUI::newDelegate(this, &ImageListControl::notifyListChangePosition);

		DataSelectorManager::getInstance().getEvent("Root")->connect(this, &ImageListControl::notifyChangeDataSelector);
		mParentData = DataManager::getInstance().getSelectedDataByType("Root");
		notifyChangeDataSelector(mParentData, false);
	}

	bool ImageListControl::checkCommand(bool _result)
	{
		if (_result)
			return false;

		if (mParentData == nullptr)
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

		ActionCreateData* command = new ActionCreateData();
		command->setName(MyGUI::utility::toString("item ", mNameIndex));
		command->setParent(mParentData);

		ActionManager::getInstance().doAction(command);

		mNameIndex ++;

		_result = true;
	}

	void ImageListControl::commandDestroyData(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand(_result))
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
		if (!checkCommand(_result))
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

	void ImageListControl::notifyListChangePosition(MyGUI::ListBox* _sender, size_t _index)
	{
		/*if (mLastIndex != MyGUI::ITEM_NONE && _index != MyGUI::ITEM_NONE)
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
		}*/

		Data* selection = _index != MyGUI::ITEM_NONE ? *mListBox->getItemDataAt<Data*>(_index) : nullptr;
		DataSelectorManager::getInstance().changeParentSelection(mParentData, selection);
	}

	void ImageListControl::invalidateList()
	{
		mListBox->setIndexSelected(MyGUI::ITEM_NONE);

		if (mParentData != nullptr)
		{
			for (size_t index = 0; index < mListBox->getItemCount(); index ++)
				mListBox->setItemDataAt(index, nullptr);

			const Data::VectorData& childs = mParentData->getChilds();

			while (mListBox->getItemCount() > childs.size())
				mListBox->removeItemAt(mListBox->getItemCount() - 1);

			while (mListBox->getItemCount() < childs.size())
				mListBox->addItem("", nullptr);

			for (size_t index = 0; index < childs.size(); index ++)
			{
				Data* child = childs.at(index);
				mListBox->setItemNameAt(index, child->getPropertyValue("Name"));
				mListBox->setItemDataAt(index, child);
			}
		}
		else
		{
			mListBox->removeAllItems();
		}
	}

	void ImageListControl::invalidateSelection()
	{
		size_t currentIndex = mListBox->getIndexSelected();
		size_t needIndex = mParentData->getChildIndex(mParentData->getChildSelected());

		if (currentIndex != needIndex)
			mListBox->setIndexSelected(needIndex);
	}

	void ImageListControl::notifyChangeDataSelector(Data* _data, bool _changeOnlySelection)
	{
		mParentData = _data;
		if (mParentData != nullptr)
		{
			if (!_changeOnlySelection)
				invalidateList();
			invalidateSelection();
		}
	}

	Data* ImageListControl::getDataSelection()
	{
		size_t index = mListBox->getIndexSelected();
		Data* selection = index != MyGUI::ITEM_NONE ? *mListBox->getItemDataAt<Data*>(index) : nullptr;
		return selection;
	}
}
