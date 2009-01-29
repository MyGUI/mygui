/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/

#include "precompiled.h"
#include "PanelItems.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "UndoManager.h"

#define ON_EXIT( CODE ) class _OnExit { public: ~_OnExit() { CODE; } } _onExit

inline const Ogre::UTFString localise(const Ogre::UTFString & _str)
{
	return MyGUI::LanguageManager::getInstance().getTag(_str);
}

PanelItems::PanelItems() : BasePanelViewItem("PanelItems.layout")
{
}

void PanelItems::initialise()
{
	mPanelCell->setCaption(localise("Items"));

	assignWidget(mEdit, "edit");
	assignWidget(mList, "list");
	assignWidget(mButtonAdd, "buttonAdd");
	assignWidget(mButtonDelete, "buttonDelete");
	assignWidget(mButtonSelect, "buttonSelect");
	mButtonAdd->eventMouseButtonClick = MyGUI::newDelegate(this, &PanelItems::notifyAddItem);
	mButtonDelete->eventMouseButtonClick = MyGUI::newDelegate(this, &PanelItems::notifyDeleteItem);
	mButtonSelect->eventMouseButtonClick = MyGUI::newDelegate(this, &PanelItems::notifySelectSheet);
	mEdit->eventEditSelectAccept = MyGUI::newDelegate(this, &PanelItems::notifyUpdateItem);
	mList->eventListChangePosition = MyGUI::newDelegate(this, &PanelItems::notifySelectItem);
}

void PanelItems::shutdown()
{
}

void PanelItems::update(MyGUI::WidgetPtr _current_widget)
{
	current_widget = _current_widget;

	WidgetStyle * widgetType = WidgetTypes::getInstance().find(_current_widget->getTypeName());
	//WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(_current_widget);

	if (widgetType->many_items)
	{
		setVisible(true);
		if (widgetType->name == "Tab") mPanelCell->setCaption(localise("Items"));
		else mPanelCell->setCaption(localise("Items"));
		syncItems(false);
		if (widgetType->name == "Tab") mButtonSelect->setVisible(true);
		else mButtonSelect->setVisible(false);
		mEdit->setCaption("");
	}
	else
	{
		setVisible(false);
	}
}

void PanelItems::notifyRectangleDoubleClick(MyGUI::WidgetPtr _sender)
{
	if (current_widget->getTypeName() == "Tab")
	{
		addSheetToTab(current_widget);
		// update strings panel
		syncItems(false);
		UndoManager::getInstance().addValue();
	}
}

void PanelItems::addSheetToTab(MyGUI::WidgetPtr _tab, Ogre::String _caption)
{
	MyGUI::TabPtr tab = _tab->castType<MyGUI::Tab>();
	MyGUI::SheetPtr sheet = tab->addItem(_caption);
	WidgetContainer * wc = new WidgetContainer("TabItem", "Default", sheet, "");
	if (!_caption.empty()) wc->mProperty.push_back(std::make_pair("Widget_Caption", _caption));
	EditorWidgets::getInstance().add(wc);
}

void PanelItems::syncItems(bool _apply, bool _add, Ogre::String _value)
{
	Ogre::String action;
	// FIXME/2 как-то громоздко и не настраиваемо...
	if (current_widget->getTypeName() == "Tab") action = "Tab_AddSheet";
	else
	{
		// for example "ComboBox_AddItem", "List_AddItem", etc...
		action = current_widget->getTypeName() + "_AddItem";
	}

	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);
	if (_apply)
	{
		if (_add)
		{
			if (action == "Tab_AddSheet")
			{
				addSheetToTab(current_widget, _value);
				UndoManager::getInstance().addValue();
			}
			else
			{
				MyGUI::WidgetManager::getInstance().parse(widgetContainer->widget, action, _value);
				widgetContainer->mProperty.push_back(std::make_pair(action, _value));
			}
		}
		else
		{
			if (action == "Tab_AddSheet")
			{
				EditorWidgets::getInstance().remove(current_widget->castType<MyGUI::Tab>()->findItemWith(_value));
			}
			else
			{
				int index = 0;
				for (StringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
				{
					if (iterProperty->first == action){
						if (iterProperty->second == _value)
						{
							widgetContainer->mProperty.erase(iterProperty);
							if (current_widget->getTypeName() == "ComboBox") current_widget->castType<MyGUI::ComboBox>()->removeItemAt(index);
							else if (current_widget->getTypeName() == "List") current_widget->castType<MyGUI::List>()->removeItemAt(index);
							else if (current_widget->getTypeName() == "MenuBar") current_widget->castType<MyGUI::MenuBar>()->removeItemAt(index);
							//else if (current_widget->getTypeName() == "Message") ->castType<MyGUI::Message>(current_widget)->deleteItem(index);
							return;
						}
						++index;
					}
				}
			}
		}
	}
	else // if !apply (if load)
	{
		mList->removeAllItems();
		if (action == "Tab_AddSheet")
		{
			MyGUI::TabPtr tab = current_widget->castType<MyGUI::Tab>();
			for (size_t i = 0; i<tab->getItemCount(); ++i) {
				mList->addItem(tab->getItemNameAt(i));
			}
		}
		else
		{
			for (StringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
			{
				if (iterProperty->first == action){
					mList->addItem(iterProperty->second);
				}
			}
		}
	}
}

void PanelItems::notifyAddItem(MyGUI::WidgetPtr _sender)
{
	syncItems(true, true, mEdit->getOnlyText());
	mList->addItem(mEdit->getOnlyText());
	UndoManager::getInstance().addValue();
}

void PanelItems::notifyDeleteItem(MyGUI::WidgetPtr _sender)
{
	size_t item = mList->getIndexSelected();
	if (MyGUI::ITEM_NONE == item) return;
	syncItems(true, false, mList->getItemNameAt(item));
	mList->removeItemAt(item);
	UndoManager::getInstance().addValue();
}

void PanelItems::notifySelectSheet(MyGUI::WidgetPtr _sender)
{
	size_t item = mList->getIndexSelected();
	if (MyGUI::ITEM_NONE == item) return;
	MyGUI::TabPtr tab = current_widget->castType<MyGUI::Tab>();
	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);

	Ogre::String action = "Tab_SelectSheet";
	Ogre::String value = MyGUI::utility::toString(item);
	MyGUI::WidgetManager::getInstance().parse(widgetContainer->widget, action, value);

	action = "Sheet_Select";
	for (size_t i = 0; i < tab->getItemCount(); ++i)
	{
		WidgetContainer * sheetContainer = EditorWidgets::getInstance().find(tab->getItemAt(i));

		if (i == item) MapSet(sheetContainer->mProperty, action, "true");
		else MapErase(sheetContainer->mProperty, action);
	}
	UndoManager::getInstance().addValue();
}

void PanelItems::notifyUpdateItem(MyGUI::EditPtr _widget)
{
	size_t item = mList->getIndexSelected();
	if (MyGUI::ITEM_NONE == item){ notifyAddItem(); return;}
	ON_EXIT(UndoManager::getInstance().addValue());
	Ogre::String action;
	Ogre::String value = mEdit->getOnlyText();
	Ogre::String lastitem = mList->getItemNameAt(item);
	mList->setItemNameAt(item, value);

	if (current_widget->getTypeName() == "Tab")
	{
		action = "Widget_Caption";
		MyGUI::TabPtr tab = current_widget->castType<MyGUI::Tab>();
		MyGUI::SheetPtr sheet = tab->getItemAt(item);
		WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(sheet);
		MyGUI::WidgetManager::getInstance().parse(sheet, "Widget_Caption", value);
		MapSet(widgetContainer->mProperty, action, value);
		return;
	}
	else
	{
		action = current_widget->getTypeName() + "_AddItem";
	}

	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);
	int index = 0;
	for (StringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
	{
		if (iterProperty->first == action){
			if (iterProperty->second == lastitem){
				iterProperty->second = value;
				if (current_widget->getTypeName() == "ComboBox") current_widget->castType<MyGUI::ComboBox>()->setItemNameAt(index, value);
				else if (current_widget->getTypeName() == "List") current_widget->castType<MyGUI::List>()->setItemNameAt(index, value);
				else if (current_widget->getTypeName() == "MenuBar") current_widget->castType<MyGUI::MenuBar>()->setItemNameAt(index, value);
				return;
			}
			++index;
		}
	}
}

void PanelItems::notifySelectItem(MyGUI::ListPtr _widget, size_t _position)
{
	size_t item = mList->getIndexSelected();
	if (MyGUI::ITEM_NONE == item) return;
	Ogre::String value = mList->getItemNameAt(item);
	mEdit->setOnlyText(value);
}
