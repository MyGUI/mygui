/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/

#include "precompiled.h"
#include "PanelUserData.h"
#include "WidgetContainer.h"
#include "UndoManager.h"

inline const Ogre::UTFString localise(const Ogre::UTFString & _str)
{
	return MyGUI::LanguageManager::getInstance().getTag(_str);
}

PanelUserData::PanelUserData() :
	BaseLayout("PanelUserData.layout"),
	PanelBase()
{
}

void PanelUserData::initialiseCell(PanelCell * _cell)
{
	PanelBase::initialiseCell(_cell);

	loadLayout(_cell->getClient());
	mMainWidget->setCoord(0, 0, _cell->getClient()->getWidth(), mMainWidget->getHeight());
	_cell->setCaption("UserData");

	assignWidget(mEditKey, "editKey");
	assignWidget(mEditValue, "editValue");
	assignWidget(mButtonAdd, "buttonAdd");
	assignWidget(mButtonDelete, "buttonDelete");
	assignWidget(mMultilist, "multilist");
	mButtonAdd->eventMouseButtonClick = MyGUI::newDelegate(this, &PanelUserData::notifyAddUserData);
	mButtonDelete->eventMouseButtonClick = MyGUI::newDelegate(this, &PanelUserData::notifyDeleteUserData);
	mEditKey->eventEditSelectAccept = MyGUI::newDelegate(this, &PanelUserData::notifyUpdateUserData);
	mEditValue->eventEditSelectAccept = MyGUI::newDelegate(this, &PanelUserData::notifyUpdateUserData);
	mMultilist->eventListChangePosition = MyGUI::newDelegate(this, &PanelUserData::notifySelectUserDataItem);

	const int MARGIN = 3;

	mMultilist->addColumn(localise("Key"), mEditKey->getWidth() - MARGIN);
	mMultilist->addColumn(localise("Value"), mEditKey->getWidth() - MARGIN);
}

void PanelUserData::shutdownCell()
{
	PanelBase::shutdownCell();

	BaseLayout::shutdown();
}

void PanelUserData::update(MyGUI::WidgetPtr _current_widget)
{
	int y = 0;
	current_widget = _current_widget;

	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(_current_widget);

	mMultilist->removeAllItems();
	for (StringPairs::iterator iterProperty = widgetContainer->mUserString.begin(); iterProperty != widgetContainer->mUserString.end(); ++iterProperty)
	{
		mMultilist->addItem(iterProperty->first);
		mMultilist->setSubItemNameAt(1, mMultilist->getItemCount() - 1, iterProperty->second);
	}
}

void PanelUserData::notifyAddUserData(MyGUI::WidgetPtr _sender)
{
	Ogre::String key = mEditKey->getOnlyText();
	Ogre::String value = mEditValue->getOnlyText();
	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);
	if (MapFind(widgetContainer->mUserString, key) == widgetContainer->mUserString.end())
	{
		mMultilist->addItem(key);
	}
	mMultilist->setSubItemNameAt(1, mMultilist->findSubItemWith(0, key), value);
	MapSet(widgetContainer->mUserString, key, value);
	UndoManager::getInstance().addValue();
}

void PanelUserData::notifyDeleteUserData(MyGUI::WidgetPtr _sender)
{
	size_t item = mMultilist->getItemIndexSelected();
	if (MyGUI::ITEM_NONE == item) return;

	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);
	MapErase(widgetContainer->mUserString, mMultilist->getItemNameAt(item));
	mMultilist->removeItemAt(item);
	UndoManager::getInstance().addValue();
}

void PanelUserData::notifyUpdateUserData(MyGUI::WidgetPtr _widget)
{
	size_t item = mMultilist->getItemIndexSelected();
	if (MyGUI::ITEM_NONE == item){ notifyAddUserData(); return;}
	Ogre::String key = mEditKey->getOnlyText();
	Ogre::String value = mEditValue->getOnlyText();
	Ogre::String lastkey = mMultilist->getItemNameAt(item);

	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);
	mMultilist->removeItemAt(mMultilist->findSubItemWith(0, lastkey));
	MapErase(widgetContainer->mUserString, lastkey);
	if (MapFind(widgetContainer->mUserString, key) == widgetContainer->mUserString.end())
	{
		mMultilist->addItem(key);
	}
	mMultilist->setSubItemNameAt(1, mMultilist->findSubItemWith(0, key), value);
	mMultilist->setItemSelectedAt(mMultilist->findSubItemWith(0, key));
	MapSet(widgetContainer->mUserString, key, value);
	UndoManager::getInstance().addValue();
}

void PanelUserData::notifySelectUserDataItem(MyGUI::WidgetPtr _widget, size_t _index)
{
	size_t item = mMultilist->getItemIndexSelected();
	if (MyGUI::ITEM_NONE == item) return;
	Ogre::String key = mMultilist->getSubItemNameAt(0, item);
	Ogre::String value = mMultilist->getSubItemNameAt(1, item);
	mEditKey->setOnlyText(key);
	mEditValue->setOnlyText(value);
}
