/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_PopupMenu.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_Button.h"
#include "MyGUI_MenuBar.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LayerManager.h"

#include "MyGUI_ControllerManager.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	Ogre::String PopupMenu::WidgetTypeName = "PopupMenu";

	const float POPUP_MENU_SPEED_COEF = 3.0f;

	PopupMenu::PopupMenu(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		//mWidgetClient(null),
		mHeightLine(1),
		mSubmenuImageSize(0)
	{
		// нам нужен фокус клавы
		mNeedKeyFocus = true;

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Client") {
				mWidgetClient = (*iter);
				//mWidgetClient->eventMouseButtonPressed = newDelegate(this, &List::notifyMousePressed);
			}
		}
		MYGUI_ASSERT(null != mWidgetClient, "Child Widget Client not found in skin (List must have Client)");
		mWidgetClient->eventMouseButtonReleased = newDelegate(this, &PopupMenu::notifyMouseReleased);

		// парсим свойства
		const MapString & param = _info->getParams();
		MapString::const_iterator iterS = param.find("SkinLine");
		if (iterS != param.end()) mSkinLine = iterS->second;
		MYGUI_ASSERT(false == mSkinLine.empty(), "SkinLine property or skin not found (PopupMenu must have SkinLine property)");

		iterS = param.find("HeightLine");
		if (iterS != param.end()) mHeightLine = utility::parseInt(iterS->second);
		if (mHeightLine < 1) mHeightLine = 1;
		iterS = param.find("SubmenuImageSize");
		if (iterS != param.end()) mSubmenuImageSize = utility::parseInt(iterS->second);

		// первоначально скрываем окно
		hide();
	}

	PopupMenu::ItemInfo& PopupMenu::insertItem(size_t _index, const Ogre::UTFString& _item, bool _submenu, bool _separator)
	{
		if (_index > mItems.size()) _index = mItems.size();
		ButtonPtr button = mWidgetClient->createWidget<Button>(mSkinLine, IntCoord(0, 0, mWidgetClient->getWidth(), mHeightLine), ALIGN_TOP | ALIGN_HSTRETCH);
		button->setCaption(_item);

		IntSize size = button->getTextSize();
		size.width += 7;
		button->setImageIndex(_submenu);
		button->_setInternalData(size.width);

		PopupMenuPtr submenu = NULL;
		if (_submenu)
		{
			submenu = Gui::getInstance().createWidget<PopupMenu>("PopupMenu", IntCoord(), ALIGN_DEFAULT, "Popup");
			submenu->_setOwner(this);
		}
		button->eventMouseButtonClick = newDelegate(this, &PopupMenu::notifyMouseClick);
		button->eventMouseMove = newDelegate(this, &PopupMenu::notifyOpenSubmenu);
		button->eventMouseButtonReleased = newDelegate(this, &PopupMenu::notifyMouseReleased);

		mItems.insert(mItems.begin() + _index, ItemInfo(button, _separator, submenu));

		update();
		return mItems[_index];
	}

	void PopupMenu::setItem(size_t _index, const Ogre::UTFString& _item, bool _submenu, bool _separator)
	{
		MYGUI_ASSERT(_index < mItems.size(), "index out of range");

		// если сабменю по€вилось или пропало - проще просто пересоздать
		if ((_submenu && (mItems[_index].submenu == NULL)) || (!_submenu && (mItems[_index].submenu != NULL)))
		{
			deleteItem(_index);
			insertItem(_index, _item, _submenu, _separator);
		}
		else
		{
			ButtonPtr button = mItems[_index].button;
			button->setCaption(_item);
			mItems[_index].separator = _separator;
			IntSize size = button->getTextSize();
			button->_setInternalData(size.width);

			update();
		}
	}

	void PopupMenu::deleteItem(size_t _index)
	{
		MYGUI_ASSERT(_index < mItems.size(), "index out of range");
		WidgetManager::getInstance().destroyWidget(mItems[_index].button);
		if ( mItems[_index].submenu )
			WidgetManager::getInstance().destroyWidget(mItems[_index].submenu);
		mItems.erase(mItems.begin() + _index);

		update();
	}

	void PopupMenu::deleteAllItems()
	{
		if (false == mItems.empty()) {
			WidgetManager & manager = WidgetManager::getInstance();
			for (VectorPopupMenuItemInfo::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
				manager.destroyWidget(iter->button);
				if ( iter->submenu )
					manager.destroyWidget(iter->submenu);
			}
			mItems.clear();

			update();
		}
	}

	size_t PopupMenu::getItemCount()
	{
		return mItems.size();
	}

	const Ogre::UTFString& PopupMenu::getItem(size_t _index)
	{
		MYGUI_ASSERT(_index < mItems.size(), "index out of range");
		return mItems[_index].button->getCaption();
	}

	PopupMenu::ItemInfo& PopupMenu::getItemInfo(size_t _index)
	{
		MYGUI_ASSERT(_index < mItems.size(), "index out of range");
		return mItems[_index];
	}

	void PopupMenu::update()
	{
		IntSize size;
		for (VectorPopupMenuItemInfo::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
			iter->button->setPosition(0, size.height);
			size.height += mHeightLine;
			if (iter->separator) size.height += 10;

			int width = iter->button->_getInternalData();
			if (iter->submenu != NULL) width += mSubmenuImageSize;
			if (width > size.width) size.width = width;
		}

		setSize(size + mCoord.size() - mWidgetClient->getSize());
	}

	bool PopupMenu::isRelative(WidgetPtr _widget, bool _all)
	{
		if (_widget != null)
		{
			// да, хоз€ин
			if (_widget == this->_getOwner())
				return true;
			// сына
			WidgetPtr owner = _widget->_getOwner();
			while (owner != null)
			{
				if (owner == this) return true;
				owner = owner->_getOwner();
			}
			if (_all)
			{
				owner = this->_getOwner();
				while (owner != null)
				{
					if (owner == _widget) return true;
					owner = owner->_getOwner();
				}
			}
		}
		return false;
	}

	void PopupMenu::notifyMouseClick(MyGUI::WidgetPtr _sender)
	{
		// потом передалть на интернал дата
		size_t index = ITEM_NONE;
		for (VectorPopupMenuItemInfo::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
			if (iter->button == _sender) {
				index = iter - mItems.begin();
				break;
			}
		}
		eventPopupMenuAccept(this, index);

		// делаем нажатой
		static_cast<ButtonPtr>(_sender)->setButtonPressed(true);

		// если есть сабменю, то сообщение все равно отошлем, но скрывать сами не будем
		if (mItems[index].submenu == NULL)
		{
			hidePopupMenu();
		}
	}

	void PopupMenu::notifyOpenSubmenu(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		// потом передалть на интернал дата
		size_t index = ITEM_NONE;
		for (VectorPopupMenuItemInfo::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter)
		{
			if (iter->button == _sender) {
				index = iter - mItems.begin();
				break;
			}
		}
		for (VectorPopupMenuItemInfo::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter)
		{
			if (iter->submenu)
			{
				iter->button->setButtonPressed(false);
				iter->submenu->hidePopupMenu(false);
			}
		}
		if (mItems[index].submenu)
		{
			mItems[index].button->setButtonPressed(true);
			// вычисл€ем куда ставить
			IntPoint position(getRight(), mItems[index].button->getTop() + getTop());
			if (position.left + mItems[index].submenu->getWidth() > MyGUI::Gui::getInstance().getViewWidth())
				position.left -= mItems[index].submenu->getWidth() + getWidth();
			if (position.top + mItems[index].submenu->getHeight() > MyGUI::Gui::getInstance().getViewHeight())
				position.top = mItems[index].button->getBottom() - mItems[index].submenu->getHeight() + getTop();
			mItems[index].submenu->showPopupMenu(position, false);
		}
	}

	void PopupMenu::showPopupMenu(const IntPoint& _point, bool _checkBorders)
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

		for (VectorPopupMenuItemInfo::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
			if (iter->button->getButtonPressed()) {
				iter->button->setButtonPressed(false);
			}
		}

		ControllerManager::getInstance().removeItem(this);

		ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MAX, POPUP_MENU_SPEED_COEF, true);
		ControllerManager::getInstance().addItem(this, controller);
	}

	void PopupMenu::notifyMouseReleased(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if ( this->getCoord().inside(IntPoint(_left, _top)) == false )
		{
			MyGUI::LayerItem * rootItem = null;
			MyGUI::WidgetPtr item = static_cast<MyGUI::WidgetPtr>(LayerManager::getInstance()._findLayerItem(_left, _top, rootItem));
			// провер€ем только рутовые виджеты, чтобы не провер€ть детей попапа
			while ((item != NULL) && (item->getParent() != NULL)) item = item->getParent();
			if (!isRelative(item, true))
			{
				hidePopupMenu();
				eventPopupMenuClose(this);
			}
		}
	}

	void PopupMenu::_onKeyLostFocus(WidgetPtr _new)
	{
		// не пр€тать, если фокус перешел к хоз€ину или сыну
		if (isRelative(_new)) return;
		hidePopupMenu();
		eventPopupMenuClose(this);

		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::eventKeyLostFocus(mWidgetEventSender, _new);
	}

	void PopupMenu::hidePopupMenu(bool _hideParentPopup)
	{
		if ( _hideParentPopup && (NULL != _getOwner()) )
		{
			// если наш папа попап меню или меню - спр€чем и его
			if ( _getOwner()->getWidgetType() == getWidgetType() )
			{
				castWidget<PopupMenu>(_getOwner())->hidePopupMenu();
			}
			else if ( _getOwner()->getWidgetType() == MenuBar::_getType() )
			{
				castWidget<MenuBar>(_getOwner())->resetItemSelect();
			}
		}

		// блокируем
		setEnabledSilent(false);
		// медленно скрываем
		ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MIN, POPUP_MENU_SPEED_COEF, false);
		controller->eventPostAction = newDelegate(action::actionWidgetHide);
		ControllerManager::getInstance().addItem(this, controller);

		// пр€чем всех детей
		for (VectorPopupMenuItemInfo::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter)
		{
			if (iter->submenu) {
				iter->submenu->hidePopupMenu(false);
			}
		}
	}

} // namespace MyGUI
