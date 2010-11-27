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

		mButtonAdd->eventMouseButtonClick += MyGUI::newDelegate(this, &PanelItems::notifyAddItem);
		mButtonDelete->eventMouseButtonClick += MyGUI::newDelegate(this, &PanelItems::notifyDeleteItem);
		mEdit->eventEditSelectAccept += MyGUI::newDelegate(this, &PanelItems::notifyUpdateItem);
		mList->eventListChangePosition += MyGUI::newDelegate(this, &PanelItems::notifySelectItem);

		mButtonLeft = mButtonAdd->getLeft();
		mButtonRight = mMainWidget->getWidth() - mButtonDelete->getRight();
		mButtonSpace = mButtonDelete->getLeft() - mButtonAdd->getRight();
	}

	void PanelItems::shutdown()
	{
	}

	void PanelItems::notifyChangeWidth(int _width)
	{
		int width = mMainWidget->getClientCoord().width;

		int half_width = (width - (mButtonLeft + mButtonRight + mButtonSpace)) / 2;
		mButtonAdd->setSize(half_width, mButtonAdd->getHeight());
		mButtonDelete->setCoord(mButtonAdd->getRight() + mButtonSpace, mButtonDelete->getTop(), width - (mButtonAdd->getRight() + mButtonSpace + mButtonRight), mButtonDelete->getHeight());
	}

	void PanelItems::update(MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;

		if (mCurrentWidget == nullptr)
		{
			setVisible(false);
			return;
		}

		selectItem(mCurrentWidget);

		WidgetStyle* widgetType = WidgetTypes::getInstance().findWidgetStyle(mCurrentWidget->getTypeName());

		if (widgetType->many_items)
		{
			setVisible(true);

			mPanelCell->setCaption(replaceTags("Items"));

			updateList();

			mEdit->setCaption("");
			//обновляем кнопки
			notifyChangeWidth(0);
		}
		else
		{
			setVisible(false);
		}
	}

	void PanelItems::selectItem(MyGUI::Widget* _widget)
	{
		MyGUI::IItem* item = dynamic_cast<MyGUI::IItem*>(_widget);
		if (item != nullptr && item->_getItemContainer() != nullptr)
			item->_getItemContainer()->_setItemSelected(item);

		if (_widget->getParent() != nullptr)
			selectItem(_widget->getParent());
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

	void PanelItems::updateList()
	{
		mList->removeAllItems();

		MyGUI::IItemContainer* itemContainer = dynamic_cast<MyGUI::IItemContainer*>(mCurrentWidget);

		if (itemContainer != nullptr)
		{
			size_t count = itemContainer->_getItemCount();
			for (size_t index = 0; index < count; ++ index)
				mList->addItem(itemContainer->_getItemNameAt(index));
		}
		else if (mCurrentWidget->isType<MyGUI::Tab>())
		{
			MyGUI::Tab* tab = mCurrentWidget->castType<MyGUI::Tab>();
			for (size_t i = 0; i < tab->getItemCount(); ++i)
			{
				mList->addItem(tab->getItemNameAt(i));
			}
		}
		else
		{
			std::string action = "AddItem";

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

	void PanelItems::setContainerProperty(WidgetContainer* _container, const std::string& _key, const std::string& _value)
	{
		if (_value.empty())
		{
			for (MyGUI::VectorStringPairs::iterator item = _container->mProperty.begin(); item != _container->mProperty.end(); ++ item)
			{
				if ((*item).first == _key)
				{
					_container->mProperty.erase(item);
					break;
				}
			}
		}
		else
		{
			bool found = false;
			for (MyGUI::VectorStringPairs::iterator item = _container->mProperty.begin(); item != _container->mProperty.end(); ++ item)
			{
				if ((*item).first == _key)
				{
					(*item).second = _value;
					found = true;
					break;
				}
			}

			if (!found)
				_container->mProperty.push_back(MyGUI::PairString(_key, _value));
		}
	}

	void PanelItems::addItem(const std::string& _value)
	{
		MyGUI::IItemContainer* itemContainer = dynamic_cast<MyGUI::IItemContainer*>(mCurrentWidget);

		if (itemContainer != nullptr)
		{
			itemContainer->_addItem(_value);

			MyGUI::Widget* item = itemContainer->_getItemAt(itemContainer->_getItemCount() - 1);

			if (item != nullptr)
			{
				WidgetContainer* container = new WidgetContainer(item->getTypeName(), "", item, "");
				setContainerProperty(container, "Caption", _value);
				EditorWidgets::getInstance().add(container);
			}

			UndoManager::getInstance().addValue();
		}
		else if (mCurrentWidget->isType<MyGUI::Tab>())
		{
			addSheetToTab(mCurrentWidget, _value);
			UndoManager::getInstance().addValue();
		}
		else
		{
			std::string action = "AddItem";

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
			MyGUI::Widget* item = itemContainer->_getItemAt(_index);
			itemContainer->_removeItemAt(_index);

			// при удалении виджета он сам удалит контейнер
			//EditorWidgets::getInstance().remove(item);
		}
		else if (mCurrentWidget->isType<MyGUI::Tab>())
		{
			std::string _value = mList->getItemNameAt(_index);
			MyGUI::TabItem* item = mCurrentWidget->castType<MyGUI::Tab>()->findItemWith(_value);
			EditorWidgets::getInstance().remove(item);
		}
		else
		{
			std::string _value = mList->getItemNameAt(_index);
			std::string action = "AddItem";

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
			itemContainer->_setItemNameAt(item, value);
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(itemContainer->_getItemAt(item));
			setContainerProperty(widgetContainer, "Caption", value);
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
		else
		{
			action = "AddItem";
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
					return;
				}
				++index;
			}
		}
	}

	void PanelItems::notifySelectItem(MyGUI::List* _widget, size_t _position)
	{
		size_t index = mList->getIndexSelected();
		if (MyGUI::ITEM_NONE == index)
			return;

		const MyGUI::UString& value = mList->getItemNameAt(index);
		mEdit->setOnlyText(value);

		if (mCurrentWidget->isType<MyGUI::Tab>())
		{
			mCurrentWidget->castType<MyGUI::Tab>()->setIndexSelected(index);
		}
	}

} // namespace tools
