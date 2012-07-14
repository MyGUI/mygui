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

namespace tools
{
	FACTORY_ITEM_ATTRIBUTE(ListBoxDataControl);

	ListBoxDataControl::ListBoxDataControl() :
		mListBox(nullptr),
		mParentData(nullptr),
		mLastIndex(MyGUI::ITEM_NONE),
		mContextMenu(nullptr)
	{
	}

	ListBoxDataControl::~ListBoxDataControl()
	{
	}

	void ListBoxDataControl::Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::Initialise(_parent, _place, _layoutName);

		mListBox = mMainWidget->castType<MyGUI::ListBox>(false);

		assignWidget(mContextMenu, "ContextMenu", false);
		mContextMenu->setVisible(false);

		if (mListBox != nullptr)
		{
			mListBox->eventListChangePosition += MyGUI::newDelegate(this, &ListBoxDataControl::notifyListChangePosition);
			mListBox->eventNotifyItem += MyGUI::newDelegate(this, &ListBoxDataControl::notifyItem);

			mPropertyForName = mMainWidget->getUserString("PropertyForName");
			if (mPropertyForName.empty())
				mPropertyForName = "Name";

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
				if (!mMainWidget->isUserString("DisableChangePosition"))
				{
					Data* data1 = *mListBox->getItemDataAt<Data*>(mLastIndex);
					Data* data2 = *mListBox->getItemDataAt<Data*>(_index);
					OnCommand("OnChangePositionData", std::make_pair(data1, data2));
				}
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
				mListBox->setItemNameAt(index, child->getPropertyValue(mPropertyForName));
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
		if (mParentData != nullptr)
		{
			size_t currentIndex = mListBox->getIndexSelected();
			size_t needIndex = mParentData->getChildIndex(mParentData->getChildSelected());

			if (currentIndex != needIndex)
				mListBox->setIndexSelected(needIndex);
		}
	}

	void ListBoxDataControl::notifyChangeDataSelector(Data* _data, bool _changeOnlySelection)
	{
		mParentData = _data;

		if (!_changeOnlySelection)
			invalidateList();
		invalidateSelection();
	}

	void ListBoxDataControl::notifyItem(MyGUI::ListBox* _sender, const MyGUI::IBNotifyItemData& _info)
	{
		if (_info.notify == MyGUI::IBNotifyItemData::MousePressed)
		{
			if (_info.id == MyGUI::MouseButton::Right)
			{
				mListBox->setIndexSelected(_info.index);
				mLastIndex = _info.index;

				Data* selection = _info.index != MyGUI::ITEM_NONE ? *mListBox->getItemDataAt<Data*>(_info.index) : nullptr;
				DataSelectorManager::getInstance().changeParentSelection(mParentData, selection);
			}
		}
		else if (_info.notify == MyGUI::IBNotifyItemData::MouseReleased)
		{
			if (_info.id == MyGUI::MouseButton::Right)
			{
				if (mContextMenu->getChildCount() != 0)
				{
					mContextMenu->setPosition(_info.x, _info.y);
					mContextMenu->setVisibleSmooth(true);
				}
			}
		}
	}
}
