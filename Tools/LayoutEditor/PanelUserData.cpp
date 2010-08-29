/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "precompiled.h"
#include "Common.h"
#include "Localise.h"
#include "PanelUserData.h"
#include "EditorWidgets.h"
#include "UndoManager.h"

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
		mPanelCell->setCaption("UserData");

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

		mMultilist->addColumn(replaceTags("Key"), 1);
		mMultilist->addColumn(replaceTags("Value"), 1);

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

		WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(_currentWidget);

		mMultilist->removeAllItems();
		for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mUserString.begin(); iterProperty != widgetContainer->mUserString.end(); ++iterProperty)
		{
			mMultilist->addItem(iterProperty->first);
			mMultilist->setSubItemNameAt(1, mMultilist->getItemCount() - 1, iterProperty->second);
		}
	}

	void PanelUserData::notifyChangeWidth(int _width)
	{
		const MyGUI::IntSize & size = mMultilist->getClientCoord().size();
		mMultilist->setColumnWidthAt(0, size.width / 2);
		mMultilist->setColumnWidthAt(1, size.width - (size.width / 2));

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
		WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
		if (utility::mapFind(widgetContainer->mUserString, key) == widgetContainer->mUserString.end())
		{
			mMultilist->addItem(key);
		}
		mMultilist->setSubItemNameAt(1, mMultilist->findSubItemWith(0, key), value);
		utility::mapSet(widgetContainer->mUserString, key, value);
		UndoManager::getInstance().addValue();
	}

	void PanelUserData::notifyDeleteUserData(MyGUI::Widget* _sender)
	{
		size_t item = mMultilist->getIndexSelected();
		if (MyGUI::ITEM_NONE == item) return;

		WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
		utility::mapErase(widgetContainer->mUserString, mMultilist->getItemNameAt(item));
		mMultilist->removeItemAt(item);
		UndoManager::getInstance().addValue();
	}

	void PanelUserData::notifyUpdateUserData(MyGUI::Edit* _widget)
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

		WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
		mMultilist->removeItemAt(mMultilist->findSubItemWith(0, lastkey));
		utility::mapErase(widgetContainer->mUserString, lastkey);
		if (utility::mapFind(widgetContainer->mUserString, key) == widgetContainer->mUserString.end())
		{
			mMultilist->addItem(key);
		}
		mMultilist->setSubItemNameAt(1, mMultilist->findSubItemWith(0, key), value);
		mMultilist->setIndexSelected(mMultilist->findSubItemWith(0, key));
		utility::mapSet(widgetContainer->mUserString, key, value);
		UndoManager::getInstance().addValue();
	}

	void PanelUserData::notifySelectUserDataItem(MyGUI::MultiList* _widget, size_t _index)
	{
		size_t item = mMultilist->getIndexSelected();
		if (MyGUI::ITEM_NONE == item) return;
		std::string key = mMultilist->getSubItemNameAt(0, item);
		std::string value = mMultilist->getSubItemNameAt(1, item);
		mEditKey->setOnlyText(key);
		mEditValue->setOnlyText(value);
	}

} // namespace tools
