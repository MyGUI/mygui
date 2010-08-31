/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "precompiled.h"
#include "Common.h"
#include "Localise.h"
#include "PanelItems.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "UndoManager.h"

namespace tools
{
	#define ON_EXIT( CODE ) class _OnExit { public: void dummy() const { }; ~_OnExit() { CODE; } } _onExit; _onExit.dummy()

	PanelItems::PanelItems() :
		BasePanelViewItem("PanelItems.layout"),
		mEdit(nullptr),
		mList(nullptr),
		mButtonAdd(nullptr),
		mButtonDelete(nullptr),
		mButtonSelect(nullptr),
		mCurrentWidget(nullptr),
		mButtonLeft(0),
		mButtonRight(0),
		mButtonSpace(0)
	{
	}

	void PanelItems::initialise()
	{
		mPanelCell->setCaption(replaceTags("Items"));

		assignWidget(mEdit, "edit");
		assignWidget(mList, "list");
		assignWidget(mButtonAdd, "buttonAdd");
		assignWidget(mButtonDelete, "buttonDelete");
		assignWidget(mButtonSelect, "buttonSelect");
		mButtonAdd->eventMouseButtonClick += MyGUI::newDelegate(this, &PanelItems::notifyAddItem);
		mButtonDelete->eventMouseButtonClick += MyGUI::newDelegate(this, &PanelItems::notifyDeleteItem);
		mButtonSelect->eventMouseButtonClick += MyGUI::newDelegate(this, &PanelItems::notifySelectSheet);
		mEdit->eventEditSelectAccept += MyGUI::newDelegate(this, &PanelItems::notifyUpdateItem);
		mList->eventListChangePosition += MyGUI::newDelegate(this, &PanelItems::notifySelectItem);

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

		if (mButtonSelect->getVisible())
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

	void PanelItems::update(MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;

		if (mCurrentWidget == nullptr)
		{
			setVisible(false);
			return;
		}

		WidgetStyle * widgetType = WidgetTypes::getInstance().findWidgetStyle(_currentWidget->getTypeName());
		//WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(_currentWidget);

		if (widgetType->many_items)
		{
			setVisible(true);

			if (widgetType->name == "Tab")
				mPanelCell->setCaption(replaceTags("Items"));
			else
				mPanelCell->setCaption(replaceTags("Items"));

			syncItems(false);

			if (widgetType->name == "Tab")
				mButtonSelect->setVisible(true);
			else
				mButtonSelect->setVisible(false);

			mEdit->setCaption("");
			//обновляем кнопки
			notifyChangeWidth(0);
		}
		else
		{
			setVisible(false);
		}
	}

	/*void PanelItems::notifyRectangleDoubleClick(MyGUI::Widget* _sender)
	{
		if (mCurrentWidget->getTypeName() == "Tab")
		{
			addSheetToTab(mCurrentWidget);
			// update strings panel
			syncItems(false);
			UndoManager::getInstance().addValue();
		}
	}*/

	void PanelItems::addSheetToTab(MyGUI::Widget* _tab, std::string _caption)
	{
		MyGUI::Tab* tab = _tab->castType<MyGUI::Tab>();
		MyGUI::TabItem* sheet = tab->addItem(_caption);
		WidgetContainer * wc = new WidgetContainer("TabItem", "", sheet, "");
		if (!_caption.empty()) wc->mProperty.push_back(MyGUI::PairString("Widget_Caption", _caption));
		EditorWidgets::getInstance().add(wc);
	}

	void PanelItems::syncItems(bool _apply, bool _add, std::string _value)
	{
		std::string action;
		// FIXME/2 как-то громоздко и не настраиваемо...
		if (mCurrentWidget->getTypeName() == "Tab") action = "Tab_AddSheet";
		else
		{
			// for example "ComboBox_AddItem", "List_AddItem", etc...
			action = mCurrentWidget->getTypeName() + "_AddItem";
		}

		WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
		if (_apply)
		{
			if (_add)
			{
				if (action == "Tab_AddSheet")
				{
					addSheetToTab(mCurrentWidget, _value);
					UndoManager::getInstance().addValue();
				}
				else if (mCurrentWidget->isType<MyGUI::MenuCtrl>())
				{
					mCurrentWidget->castType<MyGUI::MenuCtrl>()->addItem(_value);
					UndoManager::getInstance().addValue();
				}
				else
				{
					widgetContainer->widget->setProperty(action, _value);
					widgetContainer->mProperty.push_back(MyGUI::PairString(action, _value));
				}
			}
			else
			{
				if (action == "Tab_AddSheet")
				{
					EditorWidgets::getInstance().remove(mCurrentWidget->castType<MyGUI::Tab>()->findItemWith(_value));
				}
				else if (mCurrentWidget->isType<MyGUI::MenuCtrl>())
				{
					size_t item_index = mCurrentWidget->castType<MyGUI::MenuCtrl>()->findItemIndexWith(_value);
					if (item_index != MyGUI::ITEM_NONE)
						mCurrentWidget->castType<MyGUI::MenuCtrl>()->removeItemAt(item_index);
				}
				else
				{
					int index = 0;
					for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
					{
						if (iterProperty->first == action)
						{
							if (iterProperty->second == _value)
							{
								widgetContainer->mProperty.erase(iterProperty);
								if (mCurrentWidget->getTypeName() == "ComboBox") mCurrentWidget->castType<MyGUI::ComboBox>()->removeItemAt(index);
								else if (mCurrentWidget->getTypeName() == "List") mCurrentWidget->castType<MyGUI::List>()->removeItemAt(index);
								//else if (mCurrentWidget->getTypeName() == "MenuBar") mCurrentWidget->castType<MyGUI::MenuBar>()->removeItemAt(index);
								//else if (mCurrentWidget->getTypeName() == "Message") ->castType<MyGUI::Message>(mCurrentWidget)->deleteItem(index);
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
				MyGUI::Tab* tab = mCurrentWidget->castType<MyGUI::Tab>();
				for (size_t i = 0; i<tab->getItemCount(); ++i)
				{
					mList->addItem(tab->getItemNameAt(i));
				}
			}
			else if (mCurrentWidget->isType<MyGUI::MenuCtrl>())
			{
				MyGUI::MenuCtrl* menu = mCurrentWidget->castType<MyGUI::MenuCtrl>();
				for (size_t i = 0; i<menu->getItemCount(); ++i)
				{
					mList->addItem(menu->getItemNameAt(i));
				}
			}
			else
			{
				for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
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
		MyGUI::Tab* tab = mCurrentWidget->castType<MyGUI::Tab>();
		WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

		std::string action = "Tab_SelectSheet";
		std::string value = MyGUI::utility::toString(item);
		widgetContainer->widget->setProperty(action, value);

		action = "Sheet_Select";
		for (size_t i = 0; i < tab->getItemCount(); ++i)
		{
			WidgetContainer * sheetContainer = EditorWidgets::getInstance().find(tab->getItemAt(i));

			if (i == item)
				utility::mapSet(sheetContainer->mProperty, action, "true");
			else
				utility::mapErase(sheetContainer->mProperty, action);
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

		if (mCurrentWidget->getTypeName() == "Tab")
		{
			action = "Widget_Caption";
			MyGUI::Tab* tab = mCurrentWidget->castType<MyGUI::Tab>();
			MyGUI::TabItem* sheet = tab->getItemAt(item);
			WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(sheet);
			sheet->setProperty("Widget_Caption", value);
			utility::mapSet(widgetContainer->mProperty, action, value);
			return;
		}
		else if (mCurrentWidget->isType<MyGUI::MenuCtrl>())
		{
			MyGUI::MenuCtrl* menu = mCurrentWidget->castType<MyGUI::MenuCtrl>();
			for (size_t i = 0; i<menu->getItemCount(); ++i)
			{
				menu->setItemNameAt(i, mList->getItemNameAt(i));
			}
		}
		else
		{
			action = mCurrentWidget->getTypeName() + "_AddItem";
		}

		WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
		int index = 0;
		for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
		{
			if (iterProperty->first == action)
			{
				if (iterProperty->second == lastitem)
				{
					iterProperty->second = value;
					if (mCurrentWidget->getTypeName() == "ComboBox") mCurrentWidget->castType<MyGUI::ComboBox>()->setItemNameAt(index, value);
					else if (mCurrentWidget->getTypeName() == "List") mCurrentWidget->castType<MyGUI::List>()->setItemNameAt(index, value);
					//else if (mCurrentWidget->getTypeName() == "MenuBar") mCurrentWidget->castType<MyGUI::MenuBar>()->setItemNameAt(index, value);
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

		if (mCurrentWidget->isType<MyGUI::Tab>())
		{
			mCurrentWidget->castType<MyGUI::Tab>()->setIndexSelected(index);
		}
	}

} // namespace tools
