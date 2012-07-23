/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ListBoxDataControl.h"
#include "FactoryManager.h"
#include "DataManager.h"
#include "DataSelectorManager.h"
#include "Localise.h"
#include "DataUtility.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(ListBoxDataControl)

	ListBoxDataControl::ListBoxDataControl() :
		mListBox(nullptr),
		mHelpPanel(nullptr),
		mParentData(nullptr),
		mLastIndex(MyGUI::ITEM_NONE),
		mContextMenu(nullptr),
		mTextFieldControl(nullptr),
		mEnableChangePosition(false)
	{
	}

	ListBoxDataControl::~ListBoxDataControl()
	{
		delete mTextFieldControl;
		mTextFieldControl = nullptr;
	}

	void ListBoxDataControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		mListBox = mMainWidget->castType<MyGUI::ListBox>(false);

		assignWidget(mContextMenu, "ContextMenu", false);
		mContextMenu->setVisible(false);

		if (mListBox != nullptr)
		{
			mListBox->eventListChangePosition += MyGUI::newDelegate(this, &ListBoxDataControl::notifyListChangePosition);
			mListBox->eventNotifyItem += MyGUI::newDelegate(this, &ListBoxDataControl::notifyItem);
		}

		mTextFieldControl = new TextFieldControl();
		mTextFieldControl->Initialise();
		mTextFieldControl->eventEndDialog.connect(this, &ListBoxDataControl::notifyEndDialog);

		assignWidget(mHelpPanel, "HelpPanel", false, false);

		mColourName = "ColourError";
	}

	void ListBoxDataControl::notifyListChangePosition(MyGUI::ListBox* _sender, size_t _index)
	{
		if (mLastIndex != MyGUI::ITEM_NONE && _index != MyGUI::ITEM_NONE)
		{
			if (MyGUI::InputManager::getInstance().isControlPressed())
			{
				if (mEnableChangePosition)
				{
					Data* data1 = *mListBox->getItemDataAt<Data*>(mLastIndex);
					Data* data2 = *mListBox->getItemDataAt<Data*>(_index);
					eventChangePosition(data1, data2);
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

			const Data::VectorData& childs = DataUtility::getChildsByType(mParentData, mThisType);

			while (mListBox->getItemCount() > childs.size())
				mListBox->removeItemAt(mListBox->getItemCount() - 1);

			while (mListBox->getItemCount() < childs.size())
				mListBox->addItem("", nullptr);

			for (size_t index = 0; index < childs.size(); index ++)
			{
				Data* child = childs.at(index);

				bool unique = isDataEnabled(child);
				if (unique)
					mListBox->setItemNameAt(index, child->getPropertyValue(mPropertyForName));
				else
					mListBox->setItemNameAt(index, replaceTags(mColourName) + child->getPropertyValue(mPropertyForName));

				mListBox->setItemDataAt(index, child);

				connectToProperty(child);
			}
		}
		else
		{
			mListBox->removeAllItems();
		}

		if (mHelpPanel != nullptr)
			mHelpPanel->setVisible(mListBox->getItemCount() == 0);
	}

	void ListBoxDataControl::invalidateSelection()
	{
		if (mParentData != nullptr)
		{
			size_t currentIndex = mListBox->getIndexSelected();
			Data* selection = currentIndex != MyGUI::ITEM_NONE ? *mListBox->getItemDataAt<Data*>(currentIndex) : nullptr;

			if (selection != mParentData->getChildSelected())
				selectListItemByData(mParentData->getChildSelected());
		}
	}

	void ListBoxDataControl::selectListItemByData(Data* _data)
	{
		for (size_t index = 0; index < mListBox->getItemCount(); index ++)
		{
			Data* selection = *mListBox->getItemDataAt<Data*>(index);
			if (selection == _data)
			{
				mListBox->setIndexSelected(index);
				return;
			}
		}

		mListBox->setIndexSelected(MyGUI::ITEM_NONE);
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

				if (mParentData != nullptr)
				{
					Data* selection = _info.index != MyGUI::ITEM_NONE ? *mListBox->getItemDataAt<Data*>(_info.index) : nullptr;
					DataSelectorManager::getInstance().changeParentSelection(mParentData, selection);
				}
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

	void ListBoxDataControl::OnRenameData()
	{
		size_t index = mListBox->getIndexSelected();
		if (index != MyGUI::ITEM_NONE)
		{
			mListBox->beginToItemAt(index);
			MyGUI::Widget* widget = mListBox->getWidgetByIndex(index);
			if (widget != nullptr)
			{
				Data* data = *mListBox->getItemDataAt<Data*>(index);

				mTextFieldControl->setCaption(replaceTags("CaptionEnterName"));
				mTextFieldControl->setTextField(data->getPropertyValue(mPropertyForName));
				mTextFieldControl->setUserData(data);
				mTextFieldControl->setCoord(MyGUI::IntCoord(widget->getAbsoluteLeft(), widget->getAbsoluteTop(), widget->getWidth(), widget->getHeight()));
				mTextFieldControl->doModal();
			}
		}
	}

	void ListBoxDataControl::notifyEndDialog(Dialog* _sender, bool _result)
	{
		_sender->endModal();

		if (_result)
		{
			Data* data = *mTextFieldControl->getUserData<Data*>();
			eventChangeName(data, mTextFieldControl->getTextField());
		}
	}

	void ListBoxDataControl::setEnableChangePosition(bool _value)
	{
		mEnableChangePosition = _value;
	}

	void ListBoxDataControl::setDataInfo(const std::string& _parentType, const std::string& _thisType, const std::string& _propertyName)
	{
		mPropertyForName = _propertyName;
		mThisType = _thisType;

		DataSelectorManager::getInstance().getEvent(_parentType)->connect(this, &ListBoxDataControl::notifyChangeDataSelector);
		mParentData = DataUtility::getSelectedDataByType(_parentType);
		notifyChangeDataSelector(mParentData, false);
	}

	void ListBoxDataControl::connectToProperty(Data* _data)
	{
		Property* property = _data->getProperty(mPropertyForName);
		if (!property->eventChangeProperty.exist(this, &ListBoxDataControl::notifyChangeProperty))
			property->eventChangeProperty.connect(this, &ListBoxDataControl::notifyChangeProperty);

		for (VectorString::const_iterator name = mPropertyNamesEnable.begin(); name != mPropertyNamesEnable.end(); name ++)
		{
			property = _data->getProperty(*name);
			if (!property->eventChangeProperty.exist(this, &ListBoxDataControl::notifyChangeProperty))
				property->eventChangeProperty.connect(this, &ListBoxDataControl::notifyChangeProperty);
		}
	}

	void ListBoxDataControl::notifyChangeProperty(Property* _sender)
	{
		if (mParentData == nullptr)
			return;
		if (mParentData != _sender->getOwner()->getParent())
			return;

		for (size_t index = 0; index < mListBox->getItemCount(); index ++)
		{
			Data* data = *mListBox->getItemDataAt<Data*>(index);
			if (data == _sender->getOwner())
			{
				bool unique = isDataEnabled(data);
				if (unique)
					mListBox->setItemNameAt(index, data->getPropertyValue(mPropertyForName));
				else
					mListBox->setItemNameAt(index, replaceTags(mColourName) + data->getPropertyValue(mPropertyForName));
			}
		}
	}

	bool ListBoxDataControl::isDataEnabled(Data* _data)
	{
		for (VectorString::const_iterator name = mPropertyNamesEnable.begin(); name != mPropertyNamesEnable.end(); name ++)
		{
			if (!_data->getPropertyValue<bool>(*name))
				return false;
		}

		return true;
	}

	void ListBoxDataControl::setReplaceColourName(const std::string& _value)
	{
		mColourName = _value;
	}

	void ListBoxDataControl::addPropertyNameEnabled(const std::string& _propertyName)
	{
		mPropertyNamesEnable.push_back(_propertyName);
	}

}
