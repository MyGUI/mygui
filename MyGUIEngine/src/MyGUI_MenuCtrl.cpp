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

	//MYGUI_RTTI_CHILD_IMPLEMENT2( MenuCtrl, Widget );

	const float POPUP_MENU_SPEED_COEF = 3.0f;

	MenuCtrl::MenuCtrl(WidgetType _behaviour, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Widget(_behaviour, _coord, _align, _info, _parent, _croppedParent, _creator, _name),
		mHeightLine(1),
		mSubmenuImageSize(0),
		mShutdown(false),
		mSeparatorHeight(0),
		mAlignVert(true),
		mDistanceButton(0)
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
		mWidgetClient->eventMouseButtonReleased = newDelegate(this, &MenuCtrl::notifyMouseReleased);

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
		if (MenuItem::getClassTypeName() == _type) return addItem("");
		return Widget::baseCreateWidget(_behaviour, _type, _skin, _coord, _align, _layer, _name);
	}

	MenuItemPtr MenuCtrl::insertItemAt(size_t _index, const Ogre::UTFString & _name, MenuItemType _type, const std::string & _id, Any _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItemsInfo.size(), "MenuCtrl::insertItemAt");
		if (_index == ITEM_NONE) _index = mItemsInfo.size();

		MenuItemPtr item = mWidgetClient->createWidget<MenuItem>(getSkinByType(_type), IntCoord(0, 0, mWidgetClient->getWidth(), mHeightLine), Align::Top | Align::HStretch);
		item->eventMouseButtonClick = newDelegate(this, &MenuCtrl::notifyMouseClick);
		item->eventMouseMove = newDelegate(this, &MenuCtrl::notifyOpenSubmenu);
		item->eventMouseButtonReleased = newDelegate(this, &MenuCtrl::notifyMouseReleased);

		setButtonImageIndex(item, getIconIndexByType(_type ));

		MenuCtrlPtr submenu = null;
		/*if (_type == MenuItemType::Popup)
		{
			submenu = createWidgetRoot<MenuCtrl>(mSubMenuSkin, IntCoord(), Align::Default, mSubMenuLayer);
		}*/

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

	bool MenuCtrl::isRelative(WidgetPtr _widget, bool _all)
	{
		if (_widget != null)
		{
			// да, хозяин
			if (_widget == this->getParent())
				return true;
			// сына, внук и прочая мелюзга
			WidgetPtr owner = _widget->getParent();
			while (owner != null)
			{
				if (owner == this) return true;
				owner = owner->getParent();
			}
			if (_all)
			{
				// так это ж я!
				if (_widget == this) return true;
				 // предки
				owner = this->getParent();
				while (owner != null)
				{
					if (owner == _widget) return true;
					owner = owner->getParent();
				}
			}
		}
		return false;
	}

	void MenuCtrl::notifyMouseClick(MyGUI::WidgetPtr _sender)
	{
		//FIXME потом передалть на интернал дата
		size_t index = ITEM_NONE;
		for (VectorMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter) {
			if (iter->item == _sender) {
				index = iter - mItemsInfo.begin();
				break;
			}
		}
		// если вызвали через событие onMouseRelease и не попали
		if (index == ITEM_NONE) return;

		// сепаратор не кликать
		if (mItemsInfo[index].type == MenuItemType::Separator) return;

		//EventPair
		eventMenuCtrlAccept(this, mItemsInfo[index].item);

		// делаем нажатой
		static_cast<MenuItemPtr>(_sender)->setButtonPressed(true);

		// если есть сабменю, то сообщение все равно отошлем, но скрывать сами не будем
		if (mItemsInfo[index].submenu == null)
		{
			hideMenuCtrl();
		}
	}

	void MenuCtrl::notifyOpenSubmenu(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		// потом передалть на интернал дата
		size_t index = ITEM_NONE;
		for (VectorMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter)
		{
			if (iter->item == _sender) {
				index = iter - mItemsInfo.begin();
				break;
			}
		}
		for (VectorMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter)
		{
			if (iter->submenu)
			{
				iter->item->setButtonPressed(false);
				iter->submenu->hideMenuCtrl(false);
			}
		}
		if (mItemsInfo[index].submenu)
		{
			mItemsInfo[index].item->setButtonPressed(true);

			// вычисляем куда ставить
			IntPoint position;

			if (mAlignVert) {
				position.set(getRight(), mItemsInfo[index].item->getTop() + getTop());
				if (position.left + mItemsInfo[index].submenu->getWidth() > MyGUI::Gui::getInstance().getViewWidth())
					position.left -= mItemsInfo[index].submenu->getWidth() + getWidth();
				if (position.top + mItemsInfo[index].submenu->getHeight() > MyGUI::Gui::getInstance().getViewHeight())
					position.top = mItemsInfo[index].item->getBottom() - mItemsInfo[index].submenu->getHeight() + getTop();

			}
			else {
				position.set(getLeft(), getBottom());

			}

			mItemsInfo[index].submenu->showMenuCtrl(position, false);
		}
	}

	void MenuCtrl::showMenuCtrl(const IntPoint& _point, bool _checkBorders)
	{
		if (_checkBorders)
		{
			IntPoint point = _point;
			if (_point.left + getWidth() > MyGUI::Gui::getInstance().getViewWidth())
				point.left -= getWidth();
			if (_point.top + getHeight() > MyGUI::Gui::getInstance().getViewHeight())
				point.top -= getHeight();
		}
		setPosition(_point);
		InputManager::getInstance().setKeyFocusWidget(this);

		for (VectorMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter) {
			if (iter->item->getButtonPressed()) {
				iter->item->setButtonPressed(false);
			}
		}

		ControllerManager::getInstance().removeItem(this);

		ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MAX, POPUP_MENU_SPEED_COEF, true);
		ControllerManager::getInstance().addItem(this, controller);
	}

	void MenuCtrl::notifyMouseReleased(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		// если отжали не на той же кнопке
		if ( _sender->getAbsoluteCoord().inside(IntPoint(_left, _top)) == false )
		{
			//MyGUI::LayerItem * rootItem = null;
			MyGUI::WidgetPtr item = static_cast<MyGUI::WidgetPtr>(LayerManager::getInstance()._findLayerItem(_left, _top/*, rootItem*/));
			MyGUI::WidgetPtr button = item; // может понадобится, для вызова notifyMouseClick
			// проверяем только рутовые виджеты, чтобы не проверять детей попапа
			while ((item != null) && (item->getParent() != null)) item = item->getParent();
			if (isRelative(item, true) && (item->getTypeName() == MenuCtrl::getClassTypeName()))
			{
				item->castType<MenuCtrl>()->notifyMouseClick(button);
			}
			else
			{
				//hideMenuCtrl();
				eventMenuCtrlClose(this);
			}
		}
	}

	void MenuCtrl::onKeyLostFocus(WidgetPtr _new)
	{
		// не прятать, если фокус перешел к хозяину или сыну
		if (isRelative(_new)) return;
		hideMenuCtrl();
		eventMenuCtrlClose(this);

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::eventKeyLostFocus(mWidgetEventSender, _new);
	}


	void MenuCtrl::actionWidgetHide(WidgetPtr _widget)
	{
		_widget->hide();
		_widget->setEnabled(true);
	}

	void MenuCtrl::hideMenuCtrl(bool _hideParentPopup)
	{
		if ( _hideParentPopup && mParent != null )
		{
			// если наш папа попап меню или меню - спрячем и его
			MenuCtrlPtr popup = mParent->castType<MenuCtrl>(false);
			if (popup != null) popup->hideMenuCtrl();
			else {
				//MenuBarPtr menu = mParent->castType<MenuBar>(false);
				//if (menu != null) menu->clearItemSelected();
			}
		}

		// блокируем
		setEnabledSilent(false);
		// медленно скрываем
		ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MIN, POPUP_MENU_SPEED_COEF, false);
		controller->eventPostAction = newDelegate(this, &MenuCtrl::actionWidgetHide);
		ControllerManager::getInstance().addItem(this, controller);

		// прячем всех детей
		for (VectorMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter)
		{
			if (iter->submenu) {
				iter->submenu->hideMenuCtrl(false);
			}
		}
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

} // namespace MyGUI
