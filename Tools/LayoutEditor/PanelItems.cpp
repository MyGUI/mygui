/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "Precompiled.h"
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

		MyGUI::IItem* item = dynamic_cast<MyGUI::IItem*>(mCurrentWidget);
		if (item != nullptr && item->getItemContainer() != nullptr)
		{
			item->getItemContainer()->_setItemSelected(item);
		}

		WidgetStyle* widgetType = WidgetTypes::getInstance().findWidgetStyle(mCurrentWidget->getTypeName());

		if (widgetType->many_items)
		{
			setVisible(true);

			mPanelCell->setCaption(replaceTags("Items"));

			updateList();

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

	void PanelItems::addSheetToTab(MyGUI::Widget* _container, const std::string& _caption)
	{
		MyGUI::Tab* tab = _container->castType<MyGUI::Tab>();
		MyGUI::TabItem* sheet = tab->addItem(_caption);
		WidgetContainer* wc = new WidgetContainer("TabItem", "", sheet, "");
		if (!_caption.empty())
			wc->mProperty.push_back(MyGUI::PairString("Caption", _caption));
		EditorWidgets::getInstance().add(wc);
	}

	/*void PanelItems::addItemToMenu(MyGUI::Widget* _container, const std::string& _caption)
	{
		MyGUI::MenuCtrl* menu = _container->castType<MyGUI::MenuCtrl>();
		MyGUI::MenuItem* item = menu->addItem(_caption);

		WidgetContainer* itemContainer = new WidgetContainer("MenuItem", "", item, "");
		if (!_caption.empty())
			itemContainer->mProperty.push_back(MyGUI::PairString("Caption", _caption));
		EditorWidgets::getInstance().add(itemContainer);
	}*/

	void PanelItems::updateList()
	{
		mList->removeAllItems();

		MyGUI::IItemContainer* itemContainer = dynamic_cast<MyGUI::IItemContainer*>(mCurrentWidget);

		if (itemContainer != nullptr)
		{
			size_t count = itemContainer->getItemCount();
			for (size_t index = 0; index < count; ++ index)
				mList->addItem(itemContainer->getItemNameAt(index));
		}
		else if (mCurrentWidget->isType<MyGUI::Tab>())
		{
			MyGUI::Tab* tab = mCurrentWidget->castType<MyGUI::Tab>();
			for (size_t i = 0; i < tab->getItemCount(); ++i)
			{
				mList->addItem(tab->getItemNameAt(i));
			}
		}
		/*else if (mCurrentWidget->isType<MyGUI::MenuCtrl>())
		{
			MyGUI::MenuCtrl* menu = mCurrentWidget->castType<MyGUI::MenuCtrl>();
			for (size_t i = 0; i < menu->getItemCount(); ++i)
			{
				mList->addItem(menu->getItemNameAt(i));
			}
		}*/
		else
		{
			std::string action = mCurrentWidget->getTypeName() + "_AddItem";

			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
			for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
			{
				if (iterProperty->first == action)
				{
					mList->addItem(iterProperty->second);
				}
			}
		}
	}

	void PanelItems::addItem(const std::string& _value)
	{
		MyGUI::IItemContainer* itemContainer = dynamic_cast<MyGUI::IItemContainer*>(mCurrentWidget);

		if (itemContainer != nullptr)
		{
			itemContainer->addItem(_value);

			MyGUI::Widget* item = itemContainer->getItemAt(itemContainer->getItemCount() - 1);

			if (item != nullptr)
			{
				WidgetContainer* container = new WidgetContainer(item->getTypeName(), "", item, "");
				if (!_value.empty())
					container->mProperty.push_back(MyGUI::PairString("Caption", _value));
				EditorWidgets::getInstance().add(container);
			}

			UndoManager::getInstance().addValue();
		}
		else if (mCurrentWidget->isType<MyGUI::Tab>())
		{
			addSheetToTab(mCurrentWidget, _value);
			UndoManager::getInstance().addValue();
		}
		/*else if (mCurrentWidget->isType<MyGUI::MenuCtrl>())
		{
			addItemToMenu(mCurrentWidget, _value);
			UndoManager::getInstance().addValue();
		}*/
		else
		{
			std::string action = mCurrentWidget->getTypeName() + "_AddItem";

			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
			widgetContainer->widget->setProperty(action, _value);
			widgetContainer->mProperty.push_back(MyGUI::PairString(action, _value));
		}
	}

	void PanelItems::removeItem(size_t _index)
	{
		MyGUI::IItemContainer* itemContainer = dynamic_cast<MyGUI::IItemContainer*>(mCurrentWidget);

		if (itemContainer != nullptr)
		{
			MyGUI::Widget* item = itemContainer->getItemAt(_index);
			EditorWidgets::getInstance().remove(item);

			itemContainer->removeItemAt(_index);
		}
		else if (mCurrentWidget->isType<MyGUI::Tab>())
		{
			std::string _value = mList->getItemNameAt(_index);
			MyGUI::TabItem* item = mCurrentWidget->castType<MyGUI::Tab>()->findItemWith(_value);
			EditorWidgets::getInstance().remove(item);
		}
		/*else if (mCurrentWidget->isType<MyGUI::MenuCtrl>())
		{
			size_t item_index = mCurrentWidget->castType<MyGUI::MenuCtrl>()->findItemIndexWith(_value);
			if (item_index != MyGUI::ITEM_NONE)
				mCurrentWidget->castType<MyGUI::MenuCtrl>()->removeItemAt(item_index);
		}*/
		else
		{
			std::string _value = mList->getItemNameAt(_index);
			std::string action = mCurrentWidget->getTypeName() + "_AddItem";

			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
			int index = 0;
			for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
			{
				if (iterProperty->first == action)
				{
					if (iterProperty->second == _value)
					{
						widgetContainer->mProperty.erase(iterProperty);
						if (mCurrentWidget->isType<MyGUI::ComboBox>()) mCurrentWidget->castType<MyGUI::ComboBox>()->removeItemAt(index);
						else if (mCurrentWidget->isType<MyGUI::List>()) mCurrentWidget->castType<MyGUI::List>()->removeItemAt(index);
						//else if (mCurrentWidget->getTypeName() == "MenuBar") mCurrentWidget->castType<MyGUI::MenuBar>()->removeItemAt(index);
						//else if (mCurrentWidget->getTypeName() == "Message") ->castType<MyGUI::Message>(mCurrentWidget)->deleteItem(index);
						return;
					}
					++index;
				}
			}
		}
	}

	void PanelItems::notifyAddItem(MyGUI::Widget* _sender)
	{
		addItem(mEdit->getOnlyText());
		mList->addItem(mEdit->getOnlyText());
		UndoManager::getInstance().addValue();
	}

	void PanelItems::notifyDeleteItem(MyGUI::Widget* _sender)
	{
		size_t item = mList->getIndexSelected();
		if (MyGUI::ITEM_NONE == item)
			return;

		removeItem(item);
		mList->removeItemAt(item);
		UndoManager::getInstance().addValue();
	}

	void PanelItems::notifySelectSheet(MyGUI::Widget* _sender)
	{
		size_t item = mList->getIndexSelected();
		if (MyGUI::ITEM_NONE == item) return;
		MyGUI::Tab* tab = mCurrentWidget->castType<MyGUI::Tab>();
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

		std::string action = "Tab_SelectSheet";
		std::string value = MyGUI::utility::toString(item);
		widgetContainer->widget->setProperty(action, value);

		action = "Sheet_Select";
		for (size_t i = 0; i < tab->getItemCount(); ++i)
		{
			WidgetContainer* sheetContainer = EditorWidgets::getInstance().find(tab->getItemAt(i));

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
		if (MyGUI::ITEM_NONE == item)
		{
			notifyAddItem();
			return;
		}

		ON_EXIT(UndoManager::getInstance().addValue());
		std::string action;
		std::string value = mEdit->getOnlyText();
		std::string lastitem = mList->getItemNameAt(item);
		mList->setItemNameAt(item, value);

		MyGUI::IItemContainer* itemContainer = dynamic_cast<MyGUI::IItemContainer*>(mCurrentWidget);

		if (itemContainer != nullptr)
		{
			itemContainer->setItemNameAt(item, value);
			return;
		}
		else if (mCurrentWidget->isType<MyGUI::Tab>())
		{
			action = "Caption";
			MyGUI::Tab* tab = mCurrentWidget->castType<MyGUI::Tab>();
			MyGUI::TabItem* sheet = tab->getItemAt(item);
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(sheet);
			sheet->setProperty("Caption", value);
			utility::mapSet(widgetContainer->mProperty, action, value);
			return;
		}
		/*else if (mCurrentWidget->isType<MyGUI::MenuCtrl>())
		{
			MyGUI::MenuCtrl* menu = mCurrentWidget->castType<MyGUI::MenuCtrl>();
			for (size_t i = 0; i < menu->getItemCount(); ++i)
			{
				menu->setItemNameAt(i, mList->getItemNameAt(i));
			}
		}*/
		else
		{
			action = mCurrentWidget->getTypeName() + "_AddItem";
		}

		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
		int index = 0;
		for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
		{
			if (iterProperty->first == action)
			{
				if (iterProperty->second == lastitem)
				{
					iterProperty->second = value;
					if (mCurrentWidget->isType<MyGUI::ComboBox>()) mCurrentWidget->castType<MyGUI::ComboBox>()->setItemNameAt(index, value);
					else if (mCurrentWidget->isType<MyGUI::List>()) mCurrentWidget->castType<MyGUI::List>()->setItemNameAt(index, value);
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
