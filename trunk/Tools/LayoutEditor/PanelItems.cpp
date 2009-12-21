/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/

#include "precompiled.h"
#include "Common.h"
#include "PanelItems.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "UndoManager.h"

#define ON_EXIT( CODE ) class _OnExit { public: ~_OnExit() { CODE; } } _onExit

PanelItems::PanelItems() :
	BasePanelViewItem("PanelItems.layout"),
	mButtonSpace(nullptr),
	mButtonRight(nullptr),
	mButtonLeft(nullptr)
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

	mButtonLeft = mButtonAdd->getLeft();
	mButtonRight = mMainWidget->getWidth() - mButtonSelect->getRight();
	mButtonSpace = mButtonDelete->getLeft() - mButtonAdd->getRight();
}

void PanelItems::shutdown()
{
}

void PanelItems::notifyChangeWidth(int _width)
{
	int width = mMainWidget->getClientCoord().width;

	if (mButtonSelect->isVisible())
	{
		int one_width = (width - (mButtonLeft + mButtonRight + mButtonSpace)) / 3;
		mButtonAdd->setSize(one_width, mButtonAdd->getHeight());
		mButtonDelete->setCoord(mButtonAdd->getRight() + mButtonSpace, mButtonDelete->getTop(), one_width, mButtonDelete->getHeight());
		mButtonSelect->setCoord(mButtonDelete->getRight() + mButtonSpace, mButtonSelect->getTop(), width - (mButtonDelete->getRight() + mButtonSpace + mButtonRight), mButtonSelect->getHeight());
	}
	else
	{
		int half_width = (width - (mButtonLeft + mButtonRight + mButtonSpace)) / 2;
		mButtonAdd->setSize(half_width, mButtonAdd->getHeight());
		mButtonDelete->setCoord(mButtonAdd->getRight() + mButtonSpace, mButtonDelete->getTop(), width - (mButtonAdd->getRight() + mButtonSpace + mButtonRight), mButtonDelete->getHeight());
	}
}

void PanelItems::update(MyGUI::Widget* _current_widget)
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
		//обновляем кнопки
		notifyChangeWidth(0);
	}
	else
	{
		setVisible(false);
	}
}

void PanelItems::notifyRectangleDoubleClick(MyGUI::Widget* _sender)
{
	if (current_widget->getTypeName() == "Tab")
	{
		addSheetToTab(current_widget);
		// update strings panel
		syncItems(false);
		UndoManager::getInstance().addValue();
	}
}

void PanelItems::addSheetToTab(MyGUI::Widget* _tab, std::string _caption)
{
	MyGUI::Tab* tab = _tab->castType<MyGUI::Tab>();
	MyGUI::TabItem* sheet = tab->addItem(_caption);
	WidgetContainer * wc = new WidgetContainer("TabItem", "", sheet, "");
	if (!_caption.empty()) wc->mProperty.push_back(std::make_pair("Widget_Caption", _caption));
	EditorWidgets::getInstance().add(wc);
}

void PanelItems::syncItems(bool _apply, bool _add, std::string _value)
{
	std::string action;
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
			else if (current_widget->isType<MyGUI::MenuCtrl>())
			{
				current_widget->castType<MyGUI::MenuCtrl>()->addItem(_value);
				UndoManager::getInstance().addValue();
			}
			else
			{
				widgetContainer->widget->setProperty(action, _value);
				widgetContainer->mProperty.push_back(std::make_pair(action, _value));
			}
		}
		else
		{
			if (action == "Tab_AddSheet")
			{
				EditorWidgets::getInstance().remove(current_widget->castType<MyGUI::Tab>()->findItemWith(_value));
			}
			else if (current_widget->isType<MyGUI::MenuCtrl>())
			{
				size_t item_index = current_widget->castType<MyGUI::MenuCtrl>()->findItemIndexWith(_value);
				if (item_index != MyGUI::ITEM_NONE)
					current_widget->castType<MyGUI::MenuCtrl>()->removeItemAt(item_index);
			}
			else
			{
				int index = 0;
				for (StringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
				{
					if (iterProperty->first == action)
					{
						if (iterProperty->second == _value)
						{
							widgetContainer->mProperty.erase(iterProperty);
							if (current_widget->getTypeName() == "ComboBox") current_widget->castType<MyGUI::ComboBox>()->removeItemAt(index);
							else if (current_widget->getTypeName() == "List") current_widget->castType<MyGUI::List>()->removeItemAt(index);
							//else if (current_widget->getTypeName() == "MenuBar") current_widget->castType<MyGUI::MenuBar>()->removeItemAt(index);
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
			MyGUI::Tab* tab = current_widget->castType<MyGUI::Tab>();
			for (size_t i = 0; i<tab->getItemCount(); ++i)
			{
				mList->addItem(tab->getItemNameAt(i));
			}
		}
		else if (current_widget->isType<MyGUI::MenuCtrl>())
		{
			MyGUI::MenuCtrl* menu = current_widget->castType<MyGUI::MenuCtrl>();
			for (size_t i = 0; i<menu->getItemCount(); ++i)
			{
				mList->addItem(menu->getItemNameAt(i));
			}
		}
		else
		{
			for (StringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
			{
				if (iterProperty->first == action)
				{
					mList->addItem(iterProperty->second);
				}
			}
		}
	}
}

void PanelItems::notifyAddItem(MyGUI::Widget* _sender)
{
	syncItems(true, true, mEdit->getOnlyText());
	mList->addItem(mEdit->getOnlyText());
	UndoManager::getInstance().addValue();
}

void PanelItems::notifyDeleteItem(MyGUI::Widget* _sender)
{
	size_t item = mList->getIndexSelected();
	if (MyGUI::ITEM_NONE == item) return;
	syncItems(true, false, mList->getItemNameAt(item));
	mList->removeItemAt(item);
	UndoManager::getInstance().addValue();
}

void PanelItems::notifySelectSheet(MyGUI::Widget* _sender)
{
	size_t item = mList->getIndexSelected();
	if (MyGUI::ITEM_NONE == item) return;
	MyGUI::Tab* tab = current_widget->castType<MyGUI::Tab>();
	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);

	std::string action = "Tab_SelectSheet";
	std::string value = MyGUI::utility::toString(item);
	widgetContainer->widget->setProperty(action, value);

	action = "Sheet_Select";
	for (size_t i = 0; i < tab->getItemCount(); ++i)
	{
		WidgetContainer * sheetContainer = EditorWidgets::getInstance().find(tab->getItemAt(i));

		if (i == item) MapSet(sheetContainer->mProperty, action, "true");
		else MapErase(sheetContainer->mProperty, action);
	}
	UndoManager::getInstance().addValue();
}

void PanelItems::notifyUpdateItem(MyGUI::Edit* _widget)
{
	size_t item = mList->getIndexSelected();
	if (MyGUI::ITEM_NONE == item) { notifyAddItem(); return; }
	ON_EXIT(UndoManager::getInstance().addValue());
	std::string action;
	std::string value = mEdit->getOnlyText();
	std::string lastitem = mList->getItemNameAt(item);
	mList->setItemNameAt(item, value);

	if (current_widget->getTypeName() == "Tab")
	{
		action = "Widget_Caption";
		MyGUI::Tab* tab = current_widget->castType<MyGUI::Tab>();
		MyGUI::TabItem* sheet = tab->getItemAt(item);
		WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(sheet);
		sheet->setProperty("Widget_Caption", value);
		MapSet(widgetContainer->mProperty, action, value);
		return;
	}
	else if (current_widget->isType<MyGUI::MenuCtrl>())
	{
		MyGUI::MenuCtrl* menu = current_widget->castType<MyGUI::MenuCtrl>();
		for (size_t i = 0; i<menu->getItemCount(); ++i)
		{
			menu->setItemNameAt(i, mList->getItemNameAt(i));
		}
	}
	else
	{
		action = current_widget->getTypeName() + "_AddItem";
	}

	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);
	int index = 0;
	for (StringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
	{
		if (iterProperty->first == action)
		{
			if (iterProperty->second == lastitem)
			{
				iterProperty->second = value;
				if (current_widget->getTypeName() == "ComboBox") current_widget->castType<MyGUI::ComboBox>()->setItemNameAt(index, value);
				else if (current_widget->getTypeName() == "List") current_widget->castType<MyGUI::List>()->setItemNameAt(index, value);
				//else if (current_widget->getTypeName() == "MenuBar") current_widget->castType<MyGUI::MenuBar>()->setItemNameAt(index, value);
				return;
			}
			++index;
		}
	}
}

void PanelItems::notifySelectItem(MyGUI::List* _widget, size_t _position)
{
	size_t index = mList->getIndexSelected();
	if (MyGUI::ITEM_NONE == index) return;

	const MyGUI::UString& value = mList->getItemNameAt(index);
	mEdit->setOnlyText(value);

	if (current_widget->isType<MyGUI::Tab>())
	{
		current_widget->castType<MyGUI::Tab>()->setIndexSelected(index);
	}
}
