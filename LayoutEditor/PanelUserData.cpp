/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/

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
	mMainWidget->setPosition(0, 0, _cell->getClient()->getWidth(), mMainWidget->getHeight());
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

	mMultilist->addColumn(mEditKey->getWidth() - MARGIN, localise("Key"));
	mMultilist->addColumn(mEditKey->getWidth() - MARGIN, localise("Value"));
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

	mMultilist->deleteAllItems();
	for (StringPairs::iterator iterProperty = widgetContainer->mUserString.begin(); iterProperty != widgetContainer->mUserString.end(); ++iterProperty)
	{
		mMultilist->addItem(iterProperty->first);
		mMultilist->setSubItem(1, mMultilist->getItemCount() - 1, iterProperty->second);
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
	mMultilist->setSubItem(1, mMultilist->findItem(0, key), value);
	MapSet(widgetContainer->mUserString, key, value);
	UndoManager::getInstance().addValue();
}

void PanelUserData::notifyDeleteUserData(MyGUI::WidgetPtr _sender)
{
	size_t item = mMultilist->getItemSelect();
	if (MyGUI::ITEM_NONE == item) return;

	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);
	MapErase(widgetContainer->mUserString, mMultilist->getItem(item));
	mMultilist->deleteItem(item);
	UndoManager::getInstance().addValue();
}

void PanelUserData::notifyUpdateUserData(MyGUI::WidgetPtr _widget)
{
	size_t item = mMultilist->getItemSelect();
	if (MyGUI::ITEM_NONE == item){ notifyAddUserData(); return;}
	Ogre::String key = mEditKey->getOnlyText();
	Ogre::String value = mEditValue->getOnlyText();
	Ogre::String lastkey = mMultilist->getItem(item);

	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);
	mMultilist->deleteItem(mMultilist->findItem(0, lastkey));
	MapErase(widgetContainer->mUserString, lastkey);
	if (MapFind(widgetContainer->mUserString, key) == widgetContainer->mUserString.end())
	{
		mMultilist->addItem(key);
	}
	mMultilist->setSubItem(1, mMultilist->findItem(0, key), value);
	mMultilist->setItemSelect(mMultilist->findItem(0, key));
	MapSet(widgetContainer->mUserString, key, value);
	UndoManager::getInstance().addValue();
}

void PanelUserData::notifySelectUserDataItem(MyGUI::WidgetPtr _widget, size_t _index)
{
	size_t item = mMultilist->getItemSelect();
	if (MyGUI::ITEM_NONE == item) return;
	Ogre::String key = mMultilist->getSubItem(0, item);
	Ogre::String value = mMultilist->getSubItem(1, item);
	mEditKey->setOnlyText(key);
	mEditValue->setOnlyText(value);
}
