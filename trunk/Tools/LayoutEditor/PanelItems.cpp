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

	void PanelItems::updateList()
	{
		mList->removeAllItems();

		MyGUI::IItemContainer* itemContainer = dynamic_cast<MyGUI::IItemContainer*>(mCurrentWidget);

		if (itemContainer != nullptr)
		{
			size_t count = itemContainer->_getItemCount();
			for (size_t index = 0; index < count; ++ index)
				mList->addItem(MyGUI::TextIterator::toTagsString(itemContainer->_getItemNameAt(index)));
		}
	}

	void PanelItems::setContainerProperty(MyGUI::Widget* _widget, const std::string& _key, const std::string& _value)
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(_widget);

		if (_value.empty())
			widgetContainer->clearProperty(_key);
		else
			widgetContainer->setProperty(_key, _value);
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
				EditorWidgets::getInstance().add(container);

				setContainerProperty(item, "Caption", _value);
			}
			else
			{
				addPropertyValue(mCurrentWidget, "AddItem", _value);
			}

			UndoManager::getInstance().addValue();
		}
	}

	void PanelItems::removeItem(size_t _index)
	{
		MyGUI::IItemContainer* itemContainer = dynamic_cast<MyGUI::IItemContainer*>(mCurrentWidget);

		if (itemContainer != nullptr)
		{
			MyGUI::Widget* item = itemContainer->_getItemAt(_index);

			if (item == nullptr)
				erasePropertyValue(mCurrentWidget, _index, "AddItem");

			// при удалении виджета он сам удалит контейнер
			itemContainer->_removeItemAt(_index);
		}
	}

	void PanelItems::notifyAddItem(MyGUI::Widget* _sender)
	{
		addItem(mEdit->getOnlyText());
		mList->addItem(MyGUI::TextIterator::toTagsString(mEdit->getOnlyText()));
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

	void PanelItems::notifyUpdateItem(MyGUI::EditBox* _widget)
	{
		size_t item = mList->getIndexSelected();
		if (MyGUI::ITEM_NONE == item)
		{
			notifyAddItem();
			return;
		}

		std::string value = mEdit->getOnlyText();
		mList->setItemNameAt(item, MyGUI::TextIterator::toTagsString(value));

		MyGUI::IItemContainer* itemContainer = dynamic_cast<MyGUI::IItemContainer*>(mCurrentWidget);

		if (itemContainer != nullptr)
		{
			itemContainer->_setItemNameAt(item, value);
			MyGUI::Widget* widget = itemContainer->_getItemAt(item);
			if (widget != nullptr)
			{
				setContainerProperty(widget, "Caption", value);
			}
			else
			{
				setPropertyValue(mCurrentWidget, item, "AddItem", value);
			}

			UndoManager::getInstance().addValue();
		}
	}

	void PanelItems::notifySelectItem(MyGUI::ListBox* _widget, size_t _position)
	{
		size_t index = mList->getIndexSelected();
		if (MyGUI::ITEM_NONE == index)
			return;

		const MyGUI::UString& value = mList->getItemNameAt(index);
		mEdit->setCaption(value);

		if (mCurrentWidget->isType<MyGUI::TabControl>())
		{
			mCurrentWidget->castType<MyGUI::TabControl>()->setIndexSelected(index);
		}
	}

	void PanelItems::setPropertyValue(MyGUI::Widget* _widget, size_t _index, const std::string& _propertyName, const std::string& _propertyValue)
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(_widget);
		widgetContainer->setPropertyByIndex(_index, _propertyName, _propertyValue);
	}

	void PanelItems::erasePropertyValue(MyGUI::Widget* _widget, size_t _index, const std::string& _propertyName)
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(_widget);
		widgetContainer->clearPropertyByIndex(_index, _propertyName);
	}

	void PanelItems::addPropertyValue(MyGUI::Widget* _widget, const std::string& _propertyName, const std::string& _propertyValue)
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(_widget);
		widgetContainer->setProperty(_propertyName, _propertyValue, false);
	}

}
