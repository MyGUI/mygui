/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#include "Precompiled.h"
#include "ListBoxDataControl.h"
#include "FactoryItemAttribute.h"
#include "FactoryManager.h"
#include "DataManager.h"
#include "DataSelectorManager.h"
#include "ActionManager.h"
#include "ActionSwapData.h"

namespace tools
{
	FACTORY_ITEM_ATTRIBUTE(ListBoxDataControl);

	ListBoxDataControl::ListBoxDataControl() :
		mListBox(nullptr),
		mParentData(nullptr),
		mLastIndex(MyGUI::ITEM_NONE)
	{
	}

	ListBoxDataControl::~ListBoxDataControl()
	{
	}

	void ListBoxDataControl::Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::Initialise(_parent, _place, _layoutName);

		mListBox = mMainWidget->castType<MyGUI::ListBox>(false);

		if (mListBox != nullptr)
		{
			mListBox->eventListChangePosition += MyGUI::newDelegate(this, &ListBoxDataControl::notifyListChangePosition);

			std::string dataType = mMainWidget->getUserString("ParentDataType");
			DataSelectorManager::getInstance().getEvent(dataType)->connect(this, &ListBoxDataControl::notifyChangeDataSelector);
			mParentData = DataManager::getInstance().getSelectedDataByType(dataType);
			notifyChangeDataSelector(mParentData, false);
		}
	}

	void ListBoxDataControl::notifyListChangePosition(MyGUI::ListBox* _sender, size_t _index)
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

		Data* selection = _index != MyGUI::ITEM_NONE ? *mListBox->getItemDataAt<Data*>(_index) : nullptr;
		DataSelectorManager::getInstance().changeParentSelection(mParentData, selection);
	}

	void ListBoxDataControl::invalidateList()
	{
		mLastIndex = MyGUI::ITEM_NONE;
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

	void ListBoxDataControl::invalidateSelection()
	{
		size_t currentIndex = mListBox->getIndexSelected();
		size_t needIndex = mParentData->getChildIndex(mParentData->getChildSelected());

		if (currentIndex != needIndex)
			mListBox->setIndexSelected(needIndex);
	}

	void ListBoxDataControl::notifyChangeDataSelector(Data* _data, bool _changeOnlySelection)
	{
		mParentData = _data;
		if (mParentData != nullptr)
		{
			if (!_changeOnlySelection)
				invalidateList();
			invalidateSelection();
		}
	}
}
