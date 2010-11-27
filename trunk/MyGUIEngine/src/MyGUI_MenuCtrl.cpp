/*!
	@file
	@author		Albert Semenov
	@date		02/2008
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_MenuCtrl.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_MenuItem.h"
#include "MyGUI_StaticImage.h"
#include "MyGUI_MenuBar.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_RenderManager.h"

namespace MyGUI
{

	const float POPUP_MENU_SPEED_COEF = 3.0f;

	MenuCtrl::MenuCtrl() :
		mHideByAccept(true),
		mMenuDropMode(false),
		mIsMenuDrop(true),
		mHideByLostKey(false),
		mShutdown(false),
		mAlignVert(true),
		mDistanceButton(0),
		mPopupAccept(false),
		mOwner(nullptr),
		mAnimateSmooth(false),
		mChangeChildSkin(false),
		mClient(nullptr),
		mResizeToContent(true)
	{
	}

	void MenuCtrl::initialiseOverride()
	{
		Base::initialiseOverride();

		// инициализируем овнера
		Widget* parent = getParent();
		if (parent)
		{
			mOwner = parent->castType<MenuItem>(false);
			if (!mOwner)
			{
				Widget* client = parent;
				parent = client->getParent();
				if (parent && parent->getClientWidget())
				{
					mOwner = parent->castType<MenuItem>(false);
				}
			}
		}

		// FIXME нам нужен фокус клавы
		setNeedKeyFocus(true);

		assignWidget(mClient, "Client");
		if (mClient != nullptr)
		{
			setWidgetClient(mClient);
		}

		//OBSOLETE
		if (isUserString("SkinLine"))
		{
			mItemNormalSkin = getUserString("SkinLine");
			mItemPopupSkin = mItemNormalSkin;
		}

		if (isUserString("SeparatorSkin"))
			mItemSeparatorSkin = getUserString("SeparatorSkin");

		if (isUserString("NormalSkin"))
			mItemNormalSkin = getUserString("NormalSkin");

		if (isUserString("PopupSkin"))
			mItemPopupSkin = getUserString("PopupSkin");

		if (isUserString("DistanceButton"))
			mDistanceButton = utility::parseValue<int>(getUserString("DistanceButton"));

		//if (isUserString("AlignVert"))
			//mAlignVert = utility::parseValue<bool>(getUserString("AlignVert"));

		if (isUserString("SubMenuSkin"))
			mSubMenuSkin = getUserString("SubMenuSkin");
		if (isUserString("SubMenuLayer"))
			mSubMenuLayer = getUserString("SubMenuLayer");

		// FIXME добавленно, так как шетдаун вызывается и при смене скина
		mShutdown = false;
	}

	void MenuCtrl::shutdownOverride()
	{
		mShutdown = true;

		if (mOwner != nullptr)
			mOwner->getMenuCtrlParent()->_notifyDeletePopup(mOwner);

		Base::shutdownOverride();
	}

	void MenuCtrl::onWidgetCreated(Widget* _widget)
	{
		Base::onWidgetCreated(_widget);

		MenuItem* child = _widget->castType<MenuItem>(false);
		if (child != nullptr)
		{
			_wrapItem(child, mItemsInfo.size(), "", MenuItemType::Normal, "", Any::Null);
		}
	}

	MenuItem* MenuCtrl::insertItemAt(size_t _index, const UString& _name, MenuItemType _type, const std::string& _id, Any _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItemsInfo.size(), "MenuCtrl::insertItemAt");
		if (_index == ITEM_NONE) _index = mItemsInfo.size();

		MenuItem* item = _getClientWidget()->createWidget<MenuItem>(getSkinByType(_type), IntCoord(), Align::Default);
		_wrapItem(item, _index, _name, _type, _id, _data);

		return item;
	}

	void MenuCtrl::removeItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::removeItemAt");

		if (mItemsInfo[_index].submenu)
		{
			WidgetManager::getInstance().destroyWidget(mItemsInfo[_index].submenu);
			mItemsInfo[_index].submenu = nullptr;
		}
		WidgetManager::getInstance().destroyWidget(mItemsInfo[_index].item);
	}

	void MenuCtrl::removeAllItems()
	{
		while (mItemsInfo.size() > 0)
		{
			if (mItemsInfo.back().submenu)
			{
				WidgetManager::getInstance().destroyWidget(mItemsInfo.back().submenu);
				mItemsInfo.back().submenu = nullptr;
			}
			WidgetManager::getInstance().destroyWidget(mItemsInfo.back().item);
		}
	}

	const UString& MenuCtrl::getItemNameAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::getItemNameAt");
		return mItemsInfo[_index].name;
	}

	void MenuCtrl::setButtonImageIndex(Button* _button, size_t _index)
	{
		StaticImage* image = _button->getStaticImage();
		if (nullptr == image)
			return;

		if (image->getItemResource())
		{
			static const size_t CountIcons = 2;
			static const char* IconNames[CountIcons + 1] = { "None", "Popup", "" };
			if (_index >= CountIcons)
				_index = CountIcons;
			image->setItemName(IconNames[_index]);
		}
		else
		{
			image->setItemSelect(_index);
		}
	}

	void MenuCtrl::update()
	{
		IntSize size;

		if (mAlignVert)
		{
			for (VectorMenuItemInfo::iterator iter = mItemsInfo.begin(); iter != mItemsInfo.end(); ++iter)
			{
				IntSize contentSize = iter->item->_getContentSize();
				iter->item->setCoord(0, size.height, _getClientWidget()->getWidth(), contentSize.height);
				size.height += contentSize.height + mDistanceButton;

				if (contentSize.width > size.width)
					size.width = contentSize.width;
			}
			if (!mItemsInfo.empty())
				size.height -= mDistanceButton;
		}
		else
		{
			int maxHeight = 0;
			for (VectorMenuItemInfo::iterator iter = mItemsInfo.begin(); iter != mItemsInfo.end(); ++iter)
			{
				IntSize contentSize = iter->item->_getContentSize();
				if (maxHeight < contentSize.height)
					maxHeight = contentSize.height;
			}

			for (VectorMenuItemInfo::iterator iter = mItemsInfo.begin(); iter != mItemsInfo.end(); ++iter)
			{
				IntSize contentSize = iter->item->_getContentSize();
				iter->item->setCoord(size.width, 0, contentSize.width, maxHeight);
				size.width += contentSize.width + mDistanceButton;
			}

			if (!mItemsInfo.empty())
				size.width -= mDistanceButton;
		}

		if (mResizeToContent)
			setSize(size + mCoord.size() - _getClientWidget()->getSize());
	}

	void MenuCtrl::setItemDataAt(size_t _index, Any _data)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::setItemDataAt");
		mItemsInfo[_index].data = _data;
	}

	MenuCtrl* MenuCtrl::getItemChildAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::getItemChildAt");
		return mItemsInfo[_index].submenu;
	}

	void MenuCtrl::removeItemChildAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::removeItemChildAt");

		if (mItemsInfo[_index].submenu != nullptr)
		{
			WidgetManager::getInstance().destroyWidget(mItemsInfo[_index].submenu);
			mItemsInfo[_index].submenu = nullptr;
		}

		update();
	}

	void MenuCtrl::setItemNameAt(size_t _index, const UString& _name)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::setItemNameAt");

		mItemsInfo[_index].name = _name;
		MenuItem* item = mItemsInfo[_index].item;
		item->setCaption(_name);

		update();
	}

	void MenuCtrl::setItemIdAt(size_t _index, const std::string& _id)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::setItemIdAt");
		mItemsInfo[_index].id = _id;
	}

	const std::string& MenuCtrl::getItemIdAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::getItemIdAt");
		return mItemsInfo[_index].id;
	}

	void MenuCtrl::_notifyDeleteItem(MenuItem* _item)
	{
		// дитю меняем скин
		if (mChangeChildSkin)
			return;

		// общий шутдаун виджета
		if (mShutdown)
			return;

		size_t index = getItemIndex(_item);
		mItemsInfo.erase(mItemsInfo.begin() + index);
		update();
	}

	void MenuCtrl::_notifyDeletePopup(MenuItem* _item)
	{
		size_t index = getItemIndex(_item);
		mItemsInfo[index].submenu = nullptr;
	}

	void MenuCtrl::_notifyUpdateName(MenuItem* _item)
	{
		size_t index = getItemIndex(_item);
		mItemsInfo[index].name = _item->getCaption();

		ISubWidgetText* text = _item->getSubWidgetText();
		mItemsInfo[index].width = text ? (text->getTextSize().width + _item->getSize().width - text->getWidth()) : 0;
		update();
	}

	MenuItemType MenuCtrl::getItemTypeAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::getItemTypeAt");
		return mItemsInfo[_index].type;
	}

	void MenuCtrl::setItemTypeAt(size_t _index, MenuItemType _type)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::setItemTypeAt");
		ItemInfo& info = mItemsInfo[_index];
		if (info.type == _type)
			return;

		// сохраняем данные
		info.type = _type;

		// при смене скина дите отпишется
		mChangeChildSkin = true;
		info.item->changeWidgetSkin(getSkinByType(_type));
		mChangeChildSkin = false;

		setButtonImageIndex(info.item, getIconIndexByType(_type ));
		info.item->setCaption(info.name);

		update();
	}

	void MenuCtrl::notifyMenuCtrlAccept(MenuItem* _item)
	{
		if (mHideByAccept)
		{
			setVisibleSmooth(false);
		}
		else
		{
			InputManager::getInstance().setKeyFocusWidget(nullptr);
		}

		MenuItem* parent_item = getMenuItemParent();
		if (parent_item)
		{
			MenuCtrl* parent_ctrl = parent_item->getMenuCtrlParent();
			if (parent_ctrl)
			{
				parent_ctrl->notifyMenuCtrlAccept(_item);
			}
		}

		eventMenuCtrlAccept(this, _item);
	}

	void MenuCtrl::setItemChildVisibleAt(size_t _index, bool _visible)
	{
		_setItemChildVisibleAt(_index, _visible, true);
	}

	void MenuCtrl::_setItemChildVisibleAt(size_t _index, bool _visible, bool _smooth)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::setItemChildVisibleAt");

		if (_visible)
		{
			if (mItemsInfo[_index].submenu && mItemsInfo[_index].submenu->getItemCount())
			{
				int offset = mItemsInfo[0].item->getAbsoluteTop() - getAbsoluteTop();

				const IntCoord& coord = mItemsInfo[_index].item->getAbsoluteCoord();
				IntPoint point(getAbsoluteRect().right, coord.top - offset);

				MenuCtrl* menu = mItemsInfo[_index].submenu;

				if (mAlignVert)
				{
					if (point.left + menu->getWidth() > menu->getParentSize().width)
						point.left -= menu->getWidth() + getWidth();
					if (point.top + menu->getHeight() > menu->getParentSize().height)
						point.top -= menu->getHeight() + getHeight();
				}
				else
				{
					point.set(coord.left, getAbsoluteRect().bottom);
				}

				menu->setPosition(point);
				if (_smooth)
					menu->setVisibleSmooth(true);
				else
					menu->setVisible(true);

				MyGUI::LayerManager::getInstance().upLayerItem(menu);
			}
		}
		else
		{
			if (mItemsInfo[_index].submenu)
			{
				if (_smooth)
					mItemsInfo[_index].submenu->setVisibleSmooth(false);
				else
					mItemsInfo[_index].submenu->setVisible(false);
			}
		}
	}

	void MenuCtrl::notifyRootKeyChangeFocus(Widget* _sender, bool _focus)
	{
		MenuItem* item = _sender->castType<MenuItem>();
		if (item->getItemType() == MenuItemType::Popup)
		{
			if (_focus)
			{
				if (!mMenuDropMode || mIsMenuDrop)
				{
					item->setItemChildVisible(true);
					item->setStateSelected(true);
				}
			}
			else
			{
				item->setItemChildVisible(false);
				item->setStateSelected(false);
			}
		}
	}

	Widget* MenuCtrl::createItemChildByType(size_t _index, const std::string& _type)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::createItemChildByType");
		removeItemChildAt(_index);
		Widget* child = mItemsInfo[_index].item->createWidgetT(WidgetStyle::Popup, _type, mSubMenuSkin, IntCoord(), Align::Default, mSubMenuLayer);
		MYGUI_ASSERT(child->isType<MenuCtrl>(), "child must have MenuCtrl base type");
		return child;
	}

	void MenuCtrl::notifyMouseButtonClick(Widget* _sender)
	{
		MenuItem* item = _sender->castType<MenuItem>();
		if (mMenuDropMode)
		{
			if (mIsMenuDrop)
			{
				if (item->getItemType() == MenuItemType::Popup)
				{
					item->setStateSelected(false);
					item->setItemChildVisible(false);
					mIsMenuDrop = false;
				}
			}
			else
			{
				if (item->getItemType() == MenuItemType::Popup)
				{
					mIsMenuDrop = true;
					item->setStateSelected(true);
					item->setItemChildVisible(true);
					InputManager::getInstance().setKeyFocusWidget(item);
				}
			}
		}
		else
		{
			if ((item->getItemType() == MenuItemType::Popup && mPopupAccept) ||
				item->getItemType() == MenuItemType::Normal)
			{
				notifyMenuCtrlAccept(item);
			}
		}
	}

	void MenuCtrl::onKeyChangeRootFocus(bool _focus)
	{
		if (mMenuDropMode)
		{
			mIsMenuDrop = false;
		}
		if (!_focus && mHideByLostKey)
		{
			setVisibleSmooth(false);
			eventMenuCtrlClose(this);
		}
		Base::onKeyChangeRootFocus(_focus);
	}

	void MenuCtrl::notifyMouseSetFocus(Widget* _sender, Widget* _new)
	{
		InputManager::getInstance().setKeyFocusWidget(_sender);
	}

	void MenuCtrl::_wrapItemChild(MenuItem* _item, MenuCtrl* _widget)
	{
		// заменяем
		size_t index = getItemIndex(_item);
		if (mItemsInfo[index].submenu != nullptr)
		{
			WidgetManager::getInstance().destroyWidget(mItemsInfo[index].submenu);
			mItemsInfo[index].submenu = nullptr;
		}
		mItemsInfo[index].submenu = _widget;
		// скрываем менюшку
		mItemsInfo[index].submenu->setVisible(false);

		update();
	}

	void MenuCtrl::_wrapItem(MenuItem* _item, size_t _index, const UString& _name, MenuItemType _type, const std::string& _id, Any _data)
	{
		_item->setAlign(mAlignVert ? Align::Top | Align::HStretch : Align::Default);
		_item->eventRootKeyChangeFocus += newDelegate(this, &MenuCtrl::notifyRootKeyChangeFocus);
		_item->eventMouseButtonClick += newDelegate(this, &MenuCtrl::notifyMouseButtonClick);
		_item->eventMouseSetFocus += newDelegate(this, &MenuCtrl::notifyMouseSetFocus);

		setButtonImageIndex(_item, getIconIndexByType(_type ));

		MenuCtrl* submenu = nullptr;

		ItemInfo info = ItemInfo(_item, _name, _type, submenu, _id, _data);

		mItemsInfo.insert(mItemsInfo.begin() + _index, info);

		mChangeChildSkin = true;
		_item->changeWidgetSkin(mItemNormalSkin);
		mChangeChildSkin = false;

		// его сет капшен, обновит размер
		_item->setCaption(_name);

		update();
	}

	void MenuCtrl::setVisible(bool _visible)
	{
		if (mAnimateSmooth)
		{
			ControllerManager::getInstance().removeItem(this);
			setAlpha(ALPHA_MAX);
			setEnabledSilent(true);
			mAnimateSmooth = false;
		}

		if (_visible)
		{
			if (mOwner == nullptr && mHideByLostKey)
			{
				MyGUI::InputManager::getInstance().setKeyFocusWidget(this);
			}
		}

		Base::setVisible(_visible);
	}

	void MenuCtrl::setVisibleSmooth(bool _visible)
	{
		mAnimateSmooth = true;
		ControllerManager::getInstance().removeItem(this);

		if (_visible)
		{
			setEnabledSilent(true);
			if (!getVisible())
			{
				setAlpha(ALPHA_MIN);
				Base::setVisible(true);
			}

			ControllerFadeAlpha* controller = createControllerFadeAlpha(ALPHA_MAX, POPUP_MENU_SPEED_COEF, true);
			controller->eventPostAction += newDelegate(action::actionWidgetShow);
			ControllerManager::getInstance().addItem(this, controller);
		}
		else
		{
			setEnabledSilent(false);

			ControllerFadeAlpha* controller = createControllerFadeAlpha(ALPHA_MIN, POPUP_MENU_SPEED_COEF, false);
			controller->eventPostAction += newDelegate(action::actionWidgetHide);
			ControllerManager::getInstance().addItem(this, controller);
		}
	}

	ControllerFadeAlpha* MenuCtrl::createControllerFadeAlpha(float _alpha, float _coef, bool _enable)
	{
		ControllerItem* item = ControllerManager::getInstance().createItem(ControllerFadeAlpha::getClassTypeName());
		ControllerFadeAlpha* controller = item->castType<ControllerFadeAlpha>();

		controller->setAlpha(_alpha);
		controller->setCoef(_coef);
		controller->setEnabled(_enable);

		return controller;
	}

	MenuItem* MenuCtrl::insertItem(MenuItem* _to, const UString& _name, MenuItemType _type, const std::string& _id, Any _data)
	{
		return insertItemAt(getItemIndex(_to), _name, _type, _id, _data);
	}

	MenuItem* MenuCtrl::addItem(const UString& _name, MenuItemType _type, const std::string& _id, Any _data)
	{
		return insertItemAt(ITEM_NONE, _name, _type, _id, _data);
	}

	void MenuCtrl::removeItem(MenuItem* _item)
	{
		removeItemAt(getItemIndex(_item));
	}

	MenuItem* MenuCtrl::getItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::getItemAt");
		return mItemsInfo[_index].item;
	}

	size_t MenuCtrl::getItemIndex(MenuItem* _item)
	{
		for (size_t pos = 0; pos < mItemsInfo.size(); pos++)
		{
			if (mItemsInfo[pos].item == _item)
				return pos;
		}
		MYGUI_EXCEPT("item (" << _item << ") not found, source 'MenuCtrl::getItemIndex'");
	}

	MenuItem* MenuCtrl::findItemWith(const UString& _name)
	{
		for (size_t pos = 0; pos < mItemsInfo.size(); pos++)
		{
			if (mItemsInfo[pos].name == _name)
				return mItemsInfo[pos].item;
		}
		return nullptr;
	}

	MenuItem* MenuCtrl::getItemById(const std::string& _id)
	{
		for (size_t index = 0; index < mItemsInfo.size(); index++)
		{
			if (mItemsInfo[index].id == _id)
				return mItemsInfo[index].item;
		}
		MYGUI_EXCEPT("item id (" << _id << ") not found, source 'MenuCtrl::getItemById'");
	}

	size_t MenuCtrl::getItemIndexById(const std::string& _id)
	{
		for (size_t index = 0; index < mItemsInfo.size(); index++)
		{
			if (mItemsInfo[index].id == _id)
				return index;
		}
		MYGUI_EXCEPT("item id (" << _id << ") not found, source 'MenuCtrl::getItemById'");
	}

	MenuItem* MenuCtrl::findItemById(const std::string& _id, bool _recursive)
	{
		for (size_t index = 0; index < mItemsInfo.size(); index++)
		{
			if (mItemsInfo[index].id == _id)
				return mItemsInfo[index].item;

			if (_recursive && mItemsInfo[index].submenu != nullptr)
			{
				MenuItem* find = mItemsInfo[index].submenu->findItemById(_id, _recursive);
				if (find != nullptr)
					return find;
			}
		}
		return nullptr;
	}

	size_t MenuCtrl::findItemIndexWith(const UString& _name)
	{
		for (size_t index = 0; index < mItemsInfo.size(); index++)
		{
			if (mItemsInfo[index].name == _name)
				return index;
		}
		return ITEM_NONE;
	}

	size_t MenuCtrl::findItemIndex(MenuItem* _item)
	{
		for (size_t index = 0; index < mItemsInfo.size(); index++)
		{
			if (mItemsInfo[index].item == _item)
				return index;
		}
		return ITEM_NONE;
	}

	Widget* MenuCtrl::_getClientWidget()
	{
		return mClient == nullptr ? this : mClient;
	}

	size_t MenuCtrl::getItemCount() const
	{
		return mItemsInfo.size();
	}

	void MenuCtrl::setItemData(MenuItem* _item, Any _data)
	{
		setItemDataAt(getItemIndex(_item), _data);
	}

	void MenuCtrl::clearItemDataAt(size_t _index)
	{
		setItemDataAt(_index, Any::Null);
	}

	void MenuCtrl::clearItemData(MenuItem* _item)
	{
		clearItemDataAt(getItemIndex(_item));
	}

	void MenuCtrl::setItemId(MenuItem* _item, const std::string& _id)
	{
		setItemIdAt(getItemIndex(_item), _id);
	}

	const std::string& MenuCtrl::getItemId(MenuItem* _item)
	{
		return getItemIdAt(getItemIndex(_item));
	}

	void MenuCtrl::setItemName(MenuItem* _item, const UString& _name)
	{
		setItemNameAt(getItemIndex(_item), _name);
	}

	const UString& MenuCtrl::getItemName(MenuItem* _item)
	{
		return getItemNameAt(getItemIndex(_item));
	}

	void MenuCtrl::setItemChildVisible(MenuItem* _item, bool _visible)
	{
		setItemChildVisibleAt(getItemIndex(_item), _visible);
	}

	MenuCtrl* MenuCtrl::getItemChild(MenuItem* _item)
	{
		return getItemChildAt(getItemIndex(_item));
	}

	MenuCtrl* MenuCtrl::createItemChildAt(size_t _index)
	{
		return createItemChildTAt<MenuCtrl>(_index);
	}

	MenuCtrl* MenuCtrl::createItemChild(MenuItem* _item)
	{
		return createItemChildAt(getItemIndex(_item));
	}

	void MenuCtrl::removeItemChild(MenuItem* _item)
	{
		removeItemChildAt(getItemIndex(_item));
	}

	MenuItemType MenuCtrl::getItemType(MenuItem* _item)
	{
		return getItemTypeAt(getItemIndex(_item));
	}

	void MenuCtrl::setItemType(MenuItem* _item, MenuItemType _type)
	{
		setItemTypeAt(getItemIndex(_item), _type);
	}

	void MenuCtrl::setPopupAccept(bool _value)
	{
		mPopupAccept = _value;
	}

	bool MenuCtrl::getPopupAccept() const
	{
		return mPopupAccept;
	}

	MenuItem* MenuCtrl::getMenuItemParent()
	{
		return mOwner;
	}

	const std::string& MenuCtrl::getSkinByType(MenuItemType _type)
	{
		if (_type == MenuItemType::Popup)
			return mItemPopupSkin;
		else if (_type == MenuItemType::Separator)
			return mItemSeparatorSkin;
		return mItemNormalSkin;
	}

	size_t MenuCtrl::getIconIndexByType(MenuItemType _type)
	{
		return _type == MenuItemType::Popup ? ItemImagePopup : ItemImageNone;
	}

	MenuItemType MenuCtrl::getItemType(bool _submenu, bool _separator)
	{
		if (_submenu)
			return MenuItemType::Popup;
		else if (_separator)
			return MenuItemType::Separator;
		return MenuItemType::Normal;
	}

	size_t MenuCtrl::_getItemCount()
	{
		return getItemCount();
	}

	void MenuCtrl::_addItem(const MyGUI::UString& _name)
	{
		addItem(_name, MenuItemType::Normal);
	}

	void MenuCtrl::_removeItemAt(size_t _index)
	{
		removeItemAt(_index);

		_updateSizeForEmpty();
	}

	Widget* MenuCtrl::_getItemAt(size_t _index)
	{
		return getItemAt(_index);
	}

	void MenuCtrl::_setItemNameAt(size_t _index, const UString& _name)
	{
		setItemNameAt(_index, _name);
	}

	const UString& MenuCtrl::_getItemNameAt(size_t _index)
	{
		return getItemNameAt(_index);
	}

	void MenuCtrl::_setItemSelected(IItem* _item)
	{
		MenuItem* item = static_cast<MenuItem*>(_item);
		for (VectorMenuItemInfo::iterator iter = mItemsInfo.begin(); iter != mItemsInfo.end(); ++iter)
		{
			if ((*iter).type == MenuItemType::Popup)
			{
				(*iter).item->setStateSelected(false);

				if ((*iter).submenu != nullptr)
					(*iter).submenu->setVisible(false);
			}
		}

		if (item->getItemType() == MenuItemType::Popup)
		{
			item->setStateSelected(true);
			size_t index = getItemIndex(item);

			if (mItemsInfo[index].submenu)
			{
				int offset = mItemsInfo[0].item->getAbsoluteTop() - getAbsoluteTop();

				const IntCoord& coord = mItemsInfo[index].item->getAbsoluteCoord();
				IntPoint point(getAbsoluteRect().right, coord.top - offset);

				MenuCtrl* menu = mItemsInfo[index].submenu;

				if (mAlignVert)
				{
					if (point.left + menu->getWidth() > menu->getParentSize().width)
						point.left -= menu->getWidth() + getWidth();
					if (point.top + menu->getHeight() > menu->getParentSize().height)
						point.top -= menu->getHeight() + getHeight();
				}
				else
				{
					point.set(coord.left, getAbsoluteRect().bottom);
				}

				menu->setPosition(point);
				menu->setVisible(true);
			}

			_updateItems(index);
		}
	}

	void MenuCtrl::_updateItems(size_t _index)
	{
		if (mItemsInfo[_index].submenu != nullptr)
			mItemsInfo[_index].submenu->_updateSizeForEmpty();
	}

	void MenuCtrl::_updateSizeForEmpty()
	{
		if (mItemsInfo.size() == 0)
			setSize(100, 100);
	}

	void MenuCtrl::setAlignVert(bool _value)
	{
		mAlignVert = _value;

		update();
	}

	bool MenuCtrl::getAlignVert() const
	{
		return mAlignVert;
	}

	void MenuCtrl::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		if (_key == "AlignVert")
			setAlignVert(utility::parseValue<bool>(_value));
		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}
		eventChangeProperty(this, _key, _value);
	}

} // namespace MyGUI
