/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_MenuCtrl.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_MenuItem.h"
#include "MyGUI_StaticImage.h"
#include "MyGUI_MenuBar.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LayerManager.h"

#include "MyGUI_ControllerManager.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	const float POPUP_MENU_SPEED_COEF = 3.0f;

	MenuCtrl::MenuCtrl(WidgetType _behaviour, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Widget(_behaviour, _coord, _align, _info, _parent, _croppedParent, _creator, _name),
		mHeightLine(1),
		mSubmenuImageSize(0),
		mShutdown(false),
		mSeparatorHeight(0),
		mAlignVert(true),
		mDistanceButton(0),
		mHideByAccept(true),
		mMenuDropMode(false),
		mIsMenuDrop(true)
	{
		initialiseWidgetSkin(_info);
	}

	MenuCtrl::~MenuCtrl()
	{
		mShutdown = true;
		shutdownWidgetSkin();
	}

	void MenuCtrl::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Widget::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void MenuCtrl::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
		// нам нужен фокус клавы
		mNeedKeyFocus = true;

		for (VectorWidgetPtr::iterator iter=mWidgetChildSkin.begin(); iter!=mWidgetChildSkin.end(); ++iter) {
			if (*(*iter)->_getInternalData<std::string>() == "Client") {
				MYGUI_DEBUG_ASSERT( ! mWidgetClient, "widget already assigned");
				mWidgetClient = (*iter);
				//mWidgetClient->eventMouseButtonPressed = newDelegate(this, &List::notifyMousePressed);
			}
		}
		MYGUI_ASSERT(null != mWidgetClient, "Child Widget Client not found in skin (MenuCtrl must have Client)");
		//mWidgetClient->eventMouseButtonReleased = newDelegate(this, &MenuCtrl::notifyMouseReleased);

		// парсим свойства
		const MapString & properties = _info->getProperties();
		MapString::const_iterator iterS = properties.find("SkinLine");
		if (iterS != properties.end()) mSkinLine = iterS->second;
		MYGUI_ASSERT(false == mSkinLine.empty(), "SkinLine property not found (MenuCtrl must have SkinLine property)");

		iterS = properties.find("HeightLine");
		if (iterS != properties.end()) mHeightLine = utility::parseInt(iterS->second);
		if (mHeightLine < 1) {
			MYGUI_LOG(Warning, "MenuCtrl HeightLine can't be less thah 1. Set to 1.");
			mHeightLine = 1;
		}

		iterS = properties.find("SeparatorHeight");
		if (iterS != properties.end()) mSeparatorHeight = utility::parseInt(iterS->second);
		iterS = properties.find("SeparatorSkin");
		if (iterS != properties.end()) mSeparatorSkin = iterS->second;

		iterS = properties.find("SubmenuImageSize");
		if (iterS != properties.end()) mSubmenuImageSize = utility::parseInt(iterS->second);

		iterS = properties.find("SubMenuSkin");
		if (iterS != properties.end()) mSubMenuSkin = iterS->second;
		MYGUI_ASSERT(false == mSubMenuSkin.empty(), "SubMenuSkin property not found (MenuCtrl must have SubMenuSkin property)");

		iterS = properties.find("SubMenuLayer");
		if (iterS != properties.end()) mSubMenuLayer = iterS->second;
		MYGUI_ASSERT(false == mSubMenuLayer.empty(), "SubMenuLayer property not found (MenuCtrl must have SubMenuLayer property)");

		iterS = properties.find("AlignVert");
		if (iterS != properties.end()) mAlignVert = utility::parseBool(iterS->second);
		iterS = properties.find("DistanceButton");
		if (iterS != properties.end()) mDistanceButton = utility::parseInt(iterS->second);

		if (mSeparatorHeight < 1) mSeparatorHeight = mHeightLine;

		// первоначально скрываем окно
		hide();
	}

	void MenuCtrl::shutdownWidgetSkin()
	{
		mWidgetClient = null;
	}

	WidgetPtr MenuCtrl::baseCreateWidget(WidgetType _behaviour, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name)
	{
		WidgetPtr widget = Widget::baseCreateWidget(_behaviour, _type, _skin, _coord, _align, _layer, _name);
		MenuItemPtr child = widget->castType<MenuItem>(false);
		if (child) _wrapItem(child);
		return widget;
	}

	MenuItemPtr MenuCtrl::insertItemAt(size_t _index, const Ogre::UTFString & _name, MenuItemType _type, const std::string & _id, Any _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItemsInfo.size(), "MenuCtrl::insertItemAt");
		if (_index == ITEM_NONE) _index = mItemsInfo.size();

		MenuItemPtr item = mWidgetClient->createWidget<MenuItem>(getSkinByType(_type), IntCoord(0, 0, mWidgetClient->getWidth(), mHeightLine), Align::Top | Align::HStretch);
		item->eventRootKeyChangeFocus = newDelegate(this, &MenuCtrl::notifyRootKeyChangeFocus);
		item->eventRootMouseChangeFocus = newDelegate(this, &MenuCtrl::notifyRootMouseChangeFocus);
		item->eventMouseButtonClick = newDelegate(this, &MenuCtrl::notifyMouseButtonClick);

		setButtonImageIndex(item, getIconIndexByType(_type ));

		MenuCtrlPtr submenu = null;

		ItemInfo info = ItemInfo(item, _name, _type, submenu, _id, _data);

		mItemsInfo.insert(mItemsInfo.begin() + _index, info);

		// его сет капшен, обновит размер
		item->setCaption(_name);

		update();

		return item;
	}

	void MenuCtrl::removeItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::removeItemAt");

		if ( mItemsInfo[_index].submenu ) {
			WidgetManager::getInstance().destroyWidget(mItemsInfo[_index].submenu);
		}
		WidgetManager::getInstance().destroyWidget(mItemsInfo[_index].item);
	}

	void MenuCtrl::removeAllItems()
	{
		while (mItemsInfo.size() > 0) {
			if ( mItemsInfo.back().submenu ) {
				WidgetManager::getInstance().destroyWidget(mItemsInfo.back().submenu);
			}
			WidgetManager::getInstance().destroyWidget(mItemsInfo.back().item);
		}
	}

	const Ogre::UTFString& MenuCtrl::getItemNameAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::getItemNameAt");
		return mItemsInfo[_index].name;
	}

	void MenuCtrl::setButtonImageIndex(ButtonPtr _button, size_t _index)
	{
		StaticImagePtr image = _button->getStaticImage();
		if ( null == image ) return;
		if (image->getItemResource()) {
			static const size_t CountIcons = 2;
			static const char * IconNames[CountIcons + 1] = { "None", "Popup", "" };
			if (_index >= CountIcons) _index = CountIcons;
			image->setItemName(IconNames[_index]);
		}
		else {
			image->setItemSelect(_index);
		}
	}

	void MenuCtrl::update()
	{
		IntSize size;

		if (mAlignVert) {
			for (VectorMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter) {
				int height = iter->type == MenuItemType::Separator ? mSeparatorHeight : mHeightLine;
				iter->item->setCoord(0, size.height, iter->item->getWidth(), height);
				size.height += height + mDistanceButton;

				int width = iter->width;
				if (width > size.width) size.width = width;
			}

		}
		else {
			for (VectorMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter) {
				int width = iter->type == MenuItemType::Separator ? mSeparatorHeight : iter->width;
				iter->item->setCoord(size.width, 0, width, mHeightLine);
				size.width += width + mDistanceButton;
			}
			size.height = mHeightLine;
			size.width = mCoord.width;
		}

		setSize(size + mCoord.size() - mWidgetClient->getSize());
	}

	void MenuCtrl::setItemDataAt(size_t _index, Any _data)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::setItemDataAt");
		mItemsInfo[_index].data = _data;
	}

	MenuCtrlPtr MenuCtrl::getItemChildAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::getItemChildAt");
		return mItemsInfo[_index].submenu;
	}

	void MenuCtrl::removeItemChildAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::removeItemChildAt");

		if (mItemsInfo[_index].submenu != null) {
			WidgetManager::getInstance().destroyWidget(mItemsInfo[_index].submenu);
			mItemsInfo[_index].submenu = null;
		}

		update();
	}

	void MenuCtrl::setItemNameAt(size_t _index, const Ogre::UTFString & _name)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::setItemNameAt");

		mItemsInfo[_index].name = _name;
		MenuItemPtr item = mItemsInfo[_index].item;
		item->setCaption(_name);

		update();
	}

	void MenuCtrl::setItemIdAt(size_t _index, const std::string & _id)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::setItemIdAt");
		mItemsInfo[_index].id = _id;
	}

	const std::string & MenuCtrl::getItemIdAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::getItemIdAt");
		return mItemsInfo[_index].id;
	}

	void MenuCtrl::_notifyDeleteItem(MenuItemPtr _item)
	{
		// общий шутдаун виджета
		if (mShutdown) return;

		size_t index = getItemIndex(_item);
		mItemsInfo.erase(mItemsInfo.begin() + index);
		update();
	}

	void MenuCtrl::_notifyUpdateName(MenuItemPtr _item)
	{
		size_t index = getItemIndex(_item);
		mItemsInfo[index].name = _item->getCaption();
		mItemsInfo[index].width =
			_item->getTextSize().width + _item->getSize().width - _item->getTextCoord().width;
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
		ItemInfo & info = mItemsInfo[_index];
		if (info.type == _type) return;

		// сохраняем данные
		info.type = _type;
		info.item->changeWidgetSkin(getSkinByType(_type));
		setButtonImageIndex(info.item, getIconIndexByType(_type ));
		info.item->setCaption(info.name);
		
		update();
	}

	void MenuCtrl::notifyMenuCtrlAccept(MenuItemPtr _item)
	{
		WidgetPtr sender = this;

		WidgetManager::getInstance().addWidgetToUnlink(sender);
		eventMenuCtrlAccept(this, _item);
		WidgetManager::getInstance().removeWidgetFromUnlink(sender);

		// если мы еще живы, передаем отцу
		if (sender) {
			WidgetPtr parent = sender->getLogicalParent();
			if (parent) {
				MenuItemPtr item = parent->castType<MenuItem>(false);
				if (item) {
					parent = item->getLogicalParent();
					if (parent) {
						MenuCtrlPtr menu = parent->castType<MenuCtrl>(false);
						if (menu) {
							menu->notifyMenuCtrlAccept(_item);
						}
					}
				}
			}
		}

		if (mHideByAccept) {
			// блокируем
			setEnabledSilent(false);
			// медленно скрываем
			ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MIN, POPUP_MENU_SPEED_COEF, false);
			controller->eventPostAction = newDelegate(this, &MenuCtrl::actionWidgetHide);
			ControllerManager::getInstance().addItem(this, controller);
		}
		else
		{
			InputManager::getInstance().setKeyFocusWidget(null);
		}
	}

	void MenuCtrl::actionWidgetHide(WidgetPtr _widget)
	{
		_widget->hide();
		_widget->setEnabled(true);
		_widget->setAlpha(1);
	}

	void MenuCtrl::showItemChildAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::showItemChildAt");
		if (mItemsInfo[_index].submenu) {

			int offset = mItemsInfo[0].item->getAbsoluteTop() - this->getAbsoluteTop();

			const IntCoord& coord = mItemsInfo[_index].item->getAbsoluteCoord();
			IntPoint point(this->getAbsoluteRect().right, coord.top - offset);

			MenuCtrlPtr menu = mItemsInfo[_index].submenu;

			if (this->mAlignVert)
			{
				if (point.left + menu->getWidth() > MyGUI::Gui::getInstance().getViewWidth())
					point.left -= menu->getWidth();
				if (point.top + menu->getHeight() > MyGUI::Gui::getInstance().getViewHeight())
					point.top -= menu->getHeight();
			}
			else
			{
				point.set(coord.left, this->getAbsoluteRect().bottom);
			}

			menu->setPosition(point);
			menu->setEnabledSilent(true);

			ControllerManager::getInstance().removeItem(menu);

			ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MAX, POPUP_MENU_SPEED_COEF, true);
			ControllerManager::getInstance().addItem(menu, controller);
		}
	}

	void MenuCtrl::hideItemChildAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::hideItemChildAt");
		if (mItemsInfo[_index].submenu) {
			MenuCtrlPtr menu = mItemsInfo[_index].submenu;
			// блокируем
			menu->setEnabledSilent(false);
			// медленно скрываем
			ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MIN, POPUP_MENU_SPEED_COEF, false);
			controller->eventPostAction = newDelegate(menu, &MenuCtrl::actionWidgetHide);
			ControllerManager::getInstance().addItem(menu, controller);
		}
	}

	void MenuCtrl::notifyRootKeyChangeFocus(WidgetPtr _sender, bool _focus)
	{
		MenuItemPtr item = _sender->castType<MenuItem>();
		if (item->getItemType() == MenuItemType::Popup) {
			if (_focus)
			{
				if (!mMenuDropMode || mIsMenuDrop) {
					item->showItemChild();
					item->setButtonPressed(true);
				}
			}
			else
			{
				item->hideItemChild();
				item->setButtonPressed(false);
				/*if (mMenuDropMode) {
					mIsMenuDrop = false;
				}*/
			}
		}
	}

	void MenuCtrl::notifyRootMouseChangeFocus(WidgetPtr _sender, bool _focus)
	{
		MenuItemPtr item = _sender->castType<MenuItem>();
		if (_focus) InputManager::getInstance().setKeyFocusWidget(_sender);
	}

	WidgetPtr MenuCtrl::createItemChildByType(size_t _index, const std::string& _type)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::createItemChildByType");
		removeItemChildAt(_index);
		WidgetPtr child = mItemsInfo[_index].item->createWidgetT(WidgetType::Popup, _type, mSubMenuSkin, IntCoord(), Align::Default, mSubMenuLayer);
		MYGUI_ASSERT(child->isType<MenuCtrl>(), "дитя должно наследоваться от MenuCtrl");
		mItemsInfo[_index].submenu = static_cast<MenuCtrl*>(child);
		update();
		return child;
	}

	void MenuCtrl::notifyMouseButtonClick(WidgetPtr _sender)
	{
		MenuItemPtr item = _sender->castType<MenuItem>();
		if (!mMenuDropMode)
		{
			if (item->getItemType() == MenuItemType::Normal)
			{
				notifyMenuCtrlAccept(item);
			}
		}
		else
		{
			if (mIsMenuDrop)
			{
				if (item->getItemType() == MenuItemType::Popup) {
					item->setButtonPressed(false);
					item->hideItemChild();
					mIsMenuDrop = false;
				}
			}
			else
			{
				if (item->getItemType() == MenuItemType::Popup) {
					mIsMenuDrop = true;
					item->setButtonPressed(true);
					item->showItemChild();
					InputManager::getInstance().setKeyFocusWidget(item);
				}
			}
		}

	}

	void MenuCtrl::onKeyChangeRootFocus(bool _focus)
	{
		if (mMenuDropMode) {
			mIsMenuDrop = false;
		}
		Widget::onKeyChangeRootFocus(_focus);
	}

	void MenuCtrl::_wrapItemChild(MenuItemPtr _item, MenuCtrlPtr _widget)
	{
		// заменяем
		size_t index = getItemIndex(_item);
		if (mItemsInfo[index].submenu != null)
		{
			WidgetManager::getInstance().destroyWidget(mItemsInfo[index].submenu);
		}
		mItemsInfo[index].submenu = _widget;

		// приаттачиваем к лееру
		// FIXME
		//_widget->attachToLayer(mSubMenuLayer);

		update();
	}

	void MenuCtrl::_wrapItem(MenuItemPtr _item)
	{
	}

} // namespace MyGUI
