/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "Precompiled.h"
#include "PanelUserData.h"
#include "Common.h"
#include "Localise.h"
#include "UndoManager.h"
#include "WidgetStyle.h"
#include "WidgetTypes.h"

namespace tools
{

	PanelUserData::PanelUserData() :
		BasePanelViewItem("PanelUserData.layout"),
		mEditKey(nullptr),
		mEditValue(nullptr),
		mButtonAdd(nullptr),
		mButtonDelete(nullptr),
		mMultilist(nullptr),
		mCurrentWidget(nullptr),
		mEditLeft(0),
		mEditRight(0),
		mEditSpace(0),
		mButtonLeft(0),
		mButtonRight(0),
		mButtonSpace(0)
	{
	}

	void PanelUserData::initialise()
	{
		mPanelCell->setCaption(replaceTags("PanelUserDataName"));

		assignWidget(mEditKey, "editKey");
		assignWidget(mEditValue, "editValue");
		assignWidget(mButtonAdd, "buttonAdd");
		assignWidget(mButtonDelete, "buttonDelete");
		assignWidget(mMultilist, "multilist");

		mButtonAdd->eventMouseButtonClick += MyGUI::newDelegate(this, &PanelUserData::notifyAddUserData);
		mButtonDelete->eventMouseButtonClick += MyGUI::newDelegate(this, &PanelUserData::notifyDeleteUserData);
		mEditKey->eventEditSelectAccept += MyGUI::newDelegate(this, &PanelUserData::notifyUpdateUserData);
		mEditValue->eventEditSelectAccept += MyGUI::newDelegate(this, &PanelUserData::notifyUpdateUserData);
		mMultilist->eventListChangePosition += MyGUI::newDelegate(this, &PanelUserData::notifySelectUserDataItem);

		mEditLeft = mEditKey->getLeft();
		mEditRight = mMainWidget->getWidth() - mEditValue->getRight();
		mEditSpace = mEditValue->getLeft() - mEditKey->getRight();

		mButtonLeft = mButtonAdd->getLeft();
		mButtonRight = mMainWidget->getWidth() - mButtonDelete->getRight();
		mButtonSpace = mButtonDelete->getLeft() - mButtonAdd->getRight();
	}

	void PanelUserData::shutdown()
	{
	}

	void PanelUserData::update(MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;
		if (mCurrentWidget == nullptr)
			return;

		mMultilist->removeAllItems();

		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(_currentWidget);
		WidgetContainer::UserDataEnumerator userData = widgetContainer->getUserDataEnumerator();
		while (userData.next())
		{
			if (checkUserData(widgetContainer, userData.current().first))
			{
				mMultilist->addItem(userData.current().first);
				mMultilist->setSubItemNameAt(1, mMultilist->getItemCount() - 1, MyGUI::TextIterator::toTagsString(userData.current().second));
			}
		}
	}

	void PanelUserData::notifyChangeWidth(int _width)
	{
		int width = mMainWidget->getClientCoord().width;

		int half_width = (width - (mEditLeft + mEditRight + mEditSpace)) / 2;
		mEditKey->setSize(half_width, mEditKey->getHeight());
		mEditValue->setCoord(mEditKey->getRight() + mEditSpace, mEditValue->getTop(), width - (mEditKey->getRight() + mEditSpace + mEditRight), mEditValue->getHeight());

		half_width = (width - (mButtonLeft + mButtonRight + mButtonSpace)) / 2;
		mButtonAdd->setSize(half_width, mButtonAdd->getHeight());
		mButtonDelete->setCoord(mButtonAdd->getRight() + mButtonSpace, mButtonDelete->getTop(), width - (mButtonAdd->getRight() + mButtonSpace + mButtonRight), mButtonDelete->getHeight());
	}

	void PanelUserData::notifyAddUserData(MyGUI::Widget* _sender)
	{
		std::string key = mEditKey->getOnlyText();
		std::string value = mEditValue->getOnlyText();
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
		if (!widgetContainer->existUserData(key))
		{
			mMultilist->addItem(key);
		}
		mMultilist->setSubItemNameAt(1, mMultilist->findSubItemWith(0, key), MyGUI::TextIterator::toTagsString(value));
		widgetContainer->setUserData(key, value);
		UndoManager::getInstance().addValue();
	}

	void PanelUserData::notifyDeleteUserData(MyGUI::Widget* _sender)
	{
		size_t item = mMultilist->getIndexSelected();
		if (MyGUI::ITEM_NONE == item) return;

		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
		widgetContainer->clearUserData(mMultilist->getItemNameAt(item));
		mMultilist->removeItemAt(item);
		UndoManager::getInstance().addValue();
	}

	void PanelUserData::notifyUpdateUserData(MyGUI::EditBox* _widget)
	{
		size_t item = mMultilist->getIndexSelected();
		if (MyGUI::ITEM_NONE == item)
		{
			notifyAddUserData();
			return;
		}
		std::string key = mEditKey->getOnlyText();
		std::string value = mEditValue->getOnlyText();
		std::string lastkey = mMultilist->getItemNameAt(item);

		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
		mMultilist->removeItemAt(mMultilist->findSubItemWith(0, lastkey));
		widgetContainer->clearUserData(lastkey);
		if (!widgetContainer->existUserData(key))
		{
			mMultilist->addItem(key);
		}
		mMultilist->setSubItemNameAt(1, mMultilist->findSubItemWith(0, key), MyGUI::TextIterator::toTagsString(value));
		mMultilist->setIndexSelected(mMultilist->findSubItemWith(0, key));
		widgetContainer->setUserData(key, value);
		UndoManager::getInstance().addValue();
	}

	void PanelUserData::notifySelectUserDataItem(MyGUI::MultiListBox* _widget, size_t _index)
	{
		size_t item = mMultilist->getIndexSelected();
		if (MyGUI::ITEM_NONE == item) return;
		std::string key = mMultilist->getSubItemNameAt(0, item);
		std::string value = mMultilist->getSubItemNameAt(1, item);
		mEditKey->setOnlyText(key);
		mEditValue->setCaption(value);
	}

	bool PanelUserData::checkUserData(WidgetContainer* _widgetContainer, const std::string& _key)
	{
		if (_key == "LE_TargetWidgetType")
			return false;

		std::string widgetTypeName = _widgetContainer->getUserData("LE_TargetWidgetType");

		WidgetStyle* widgetType = WidgetTypes::getInstance().findWidgetStyle(_widgetContainer->getType());

		for (MyGUI::VectorStringPairs::iterator iter = widgetType->parameterData.begin(); iter != widgetType->parameterData.end(); ++iter)
		{
			if ((*iter).first == _key)
				return false;
		}

		if (!widgetTypeName.empty())
		{
			WidgetStyle* widgetTargetType = WidgetTypes::getInstance().findWidgetStyle(widgetTypeName);

			for (MyGUI::VectorStringPairs::iterator iter = widgetTargetType->templateData.begin(); iter != widgetTargetType->templateData.end(); ++iter)
			{
				if ((*iter).first == _key)
					return false;
			}
		}

		return true;
	}

}
