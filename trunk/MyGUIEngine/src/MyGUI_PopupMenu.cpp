/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_PopupMenu.h"
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

	MYGUI_RTTI_CHILD_IMPLEMENT( PopupMenu, Widget );

	const float POPUP_MENU_SPEED_COEF = 3.0f;

	PopupMenu::PopupMenu(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mHeightLine(1),
		mSubmenuImageSize(0),
		mShutdown(false),
		mSeparatorHeight(0),
		mAlignVert(true),
		mDistanceButton(0)
	{
		// нам нужен фокус клавы
		mNeedKeyFocus = true;

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if (*(*iter)->_getInternalData<std::string>() == "Client") {
				MYGUI_DEBUG_ASSERT( ! mWidgetClient, "widget already assigned");
				mWidgetClient = (*iter);
				//mWidgetClient->eventMouseButtonPressed = newDelegate(this, &List::notifyMousePressed);
			}
		}
		MYGUI_ASSERT(null != mWidgetClient, "Child Widget Client not found in skin (List must have Client)");
		mWidgetClient->eventMouseButtonReleased = newDelegate(this, &PopupMenu::notifyMouseReleased);

		// парсим свойства
		const MapString & properties = _info->getProperties();
		MapString::const_iterator iterS = properties.find("SkinLine");
		if (iterS != properties.end()) mSkinLine = iterS->second;
		MYGUI_ASSERT(false == mSkinLine.empty(), "SkinLine property not found (PopupMenu must have SkinLine property)");

		iterS = properties.find("HeightLine");
		if (iterS != properties.end()) mHeightLine = utility::parseInt(iterS->second);
		if (mHeightLine < 1) {
			MYGUI_LOG(Warning, "PopupMenu HeightLine can't be less thah 1. Set to 1.");
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
		MYGUI_ASSERT(false == mSubMenuSkin.empty(), "SubMenuSkin property not found (PopupMenu must have SubMenuSkin property)");

		iterS = properties.find("SubMenuLayer");
		if (iterS != properties.end()) mSubMenuLayer = iterS->second;
		MYGUI_ASSERT(false == mSubMenuLayer.empty(), "SubMenuLayer property not found (PopupMenu must have SubMenuLayer property)");

		iterS = properties.find("AlignVert");
		if (iterS != properties.end()) mAlignVert = utility::parseBool(iterS->second);
		iterS = properties.find("DistanceButton");
		if (iterS != properties.end()) mDistanceButton = utility::parseInt(iterS->second);

		if (mSeparatorHeight < 1) mSeparatorHeight = mHeightLine;

		// первоначально скрываем окно
		hide();
	}

	PopupMenu::~PopupMenu()
	{
		mShutdown = true;
		// просто очищаем список, виджеты сами удалятся
		// и вкладки при удалении себя не найдет в списке
	}

	// переопределяем для особого обслуживания страниц
	WidgetPtr PopupMenu::_createWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name)
	{
		if (MenuItem::getClassTypeName() == _type) return addItem("");
		return Widget::_createWidget(_type, _skin, _coord, _align, _layer, _name);
	}

	MenuItemPtr PopupMenu::insertItemAt(size_t _index, const Ogre::UTFString & _name, MenuItemType _type, const std::string & _id, Any _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItemsInfo.size(), "PopupMenu::insertItemAt");
		if (_index == ITEM_NONE) _index = mItemsInfo.size();

		std::string skin = _type == MenuItemType::Separator ? mSeparatorSkin : mSkinLine;

		MenuItemPtr item = mWidgetClient->createWidget<MenuItem>(skin, IntCoord(0, 0, mWidgetClient->getWidth(), mHeightLine), Align::Top | Align::HStretch);
		item->eventMouseButtonClick = newDelegate(this, &PopupMenu::notifyMouseClick);
		item->eventMouseMove = newDelegate(this, &PopupMenu::notifyOpenSubmenu);
		item->eventMouseButtonReleased = newDelegate(this, &PopupMenu::notifyMouseReleased);

		setButtonImageIndex(item, _type == MenuItemType::Popup ? ItemImagePopup : ItemImageNone);

		PopupMenuPtr submenu = null;
		if (_type == MenuItemType::Popup)
		{
			submenu = Gui::getInstance().createWidget<PopupMenu>(mSubMenuSkin, IntCoord(), Align::Default, mSubMenuLayer);
			submenu->_setOwner(this);
		}

		ItemInfo info = ItemInfo(item, _name, _type, submenu, _id, _data);
		/*IntSize size = item->getTextSize();
		size.width += 7; //FIXME
		info.width = size.width;*/


		mItemsInfo.insert(mItemsInfo.begin() + _index, info);

		// его сет капшен, обновит размер
		item->setCaption(_name);

		update();

		return item;
	}

	void PopupMenu::removeItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::removeItemAt");

		if ( mItemsInfo[_index].submenu ) {
			WidgetManager::getInstance().destroyWidget(mItemsInfo[_index].submenu);
		}
		WidgetManager::getInstance().destroyWidget(mItemsInfo[_index].item);
	}

	void PopupMenu::removeAllItems()
	{
		while (mItemsInfo.size() > 0) {
			if ( mItemsInfo.back().submenu ) {
				WidgetManager::getInstance().destroyWidget(mItemsInfo.back().submenu);
			}
			WidgetManager::getInstance().destroyWidget(mItemsInfo.back().item);
		}
	}

	const Ogre::UTFString& PopupMenu::getItemNameAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::getItemNameAt");
		return mItemsInfo[_index].name;
	}

	void PopupMenu::setButtonImageIndex(ButtonPtr _button, size_t _index)
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

	void PopupMenu::update()
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

	bool PopupMenu::isRelative(WidgetPtr _widget, bool _all)
	{
		if (_widget != null)
		{
			// да, хозяин
			if (_widget == this->_getOwner())
				return true;
			// сына, внук и прочая мелюзга
			WidgetPtr owner = _widget->_getOwner();
			while (owner != null)
			{
				if (owner == this) return true;
				owner = owner->_getOwner();
			}
			if (_all)
			{
				// так это ж я!
				if (_widget == this) return true;
				 // предки
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
		eventPopupMenuAccept.m_eventObsolete(this, index);
		eventPopupMenuAccept.m_event(this, mItemsInfo[index].item);

		// делаем нажатой
		static_cast<MenuItemPtr>(_sender)->setButtonPressed(true);

		// если есть сабменю, то сообщение все равно отошлем, но скрывать сами не будем
		if (mItemsInfo[index].submenu == null)
		{
			hidePopupMenu();
		}
	}

	void PopupMenu::notifyOpenSubmenu(MyGUI::WidgetPtr _sender, int _left, int _top)
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
				iter->submenu->hidePopupMenu(false);
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

			mItemsInfo[index].submenu->showPopupMenu(position, false);
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

		for (VectorMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter) {
			if (iter->item->getButtonPressed()) {
				iter->item->setButtonPressed(false);
			}
		}

		ControllerManager::getInstance().removeItem(this);

		ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MAX, POPUP_MENU_SPEED_COEF, true);
		ControllerManager::getInstance().addItem(this, controller);
	}

	void PopupMenu::notifyMouseReleased(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		// если отжали не на той же кнопке
		if ( _sender->getAbsoluteCoord().inside(IntPoint(_left, _top)) == false )
		{
			MyGUI::LayerItem * rootItem = null;
			MyGUI::WidgetPtr item = static_cast<MyGUI::WidgetPtr>(LayerManager::getInstance()._findLayerItem(_left, _top, rootItem));
			MyGUI::WidgetPtr button = item; // может понадобится, для вызова notifyMouseClick
			// проверяем только рутовые виджеты, чтобы не проверять детей попапа
			while ((item != null) && (item->getParent() != null)) item = item->getParent();
			if (isRelative(item, true))
			{
				item->castType<PopupMenu>()->notifyMouseClick(button);
			}
			else
			{
				hidePopupMenu();
				eventPopupMenuClose(this);
			}
		}
	}

	void PopupMenu::onKeyLostFocus(WidgetPtr _new)
	{
		// не прятать, если фокус перешел к хозяину или сыну
		if (isRelative(_new)) return;
		hidePopupMenu();
		eventPopupMenuClose(this);

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::eventKeyLostFocus(mWidgetEventSender, _new);
	}

	void PopupMenu::hidePopupMenu(bool _hideParentPopup)
	{
		if ( _hideParentPopup && mOwner != null )
		{
			// если наш папа попап меню или меню - спрячем и его
			PopupMenuPtr popup = mOwner->castType<PopupMenu>(false);
			if (popup != null) popup->hidePopupMenu();
			else {
				MenuBarPtr menu = mOwner->castType<MenuBar>(false);
				if (menu != null) menu->clearItemSelected();
			}
		}

		// блокируем
		setEnabledSilent(false);
		// медленно скрываем
		ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MIN, POPUP_MENU_SPEED_COEF, false);
		controller->eventPostAction = newDelegate(action::actionWidgetHide);
		ControllerManager::getInstance().addItem(this, controller);

		// прячем всех детей
		for (VectorMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter)
		{
			if (iter->submenu) {
				iter->submenu->hidePopupMenu(false);
			}
		}
	}

	void PopupMenu::setItemDataAt(size_t _index, Any _data)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::setItemDataAt");
		mItemsInfo[_index].data = _data;
	}

	PopupMenuPtr PopupMenu::getItemChildAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::getItemChildAt");
		return mItemsInfo[_index].submenu;
	}

	PopupMenuPtr PopupMenu::createItemChildAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::createItemChildAt");

		if (mItemsInfo[_index].submenu == null) {
			mItemsInfo[_index].submenu = Gui::getInstance().createWidget<PopupMenu>(mSubMenuSkin, IntCoord(), Align::Default, mSubMenuLayer);
			mItemsInfo[_index].submenu->_setOwner(this);
		}
		else {
			mItemsInfo[_index].submenu->removeAllItems();
		}

		update();

		return mItemsInfo[_index].submenu;
	}

	void PopupMenu::removeItemChildAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::removeItemChildAt");

		if (mItemsInfo[_index].submenu != null) {
			WidgetManager::getInstance().destroyWidget(mItemsInfo[_index].submenu);
			mItemsInfo[_index].submenu = null;
		}

		update();
	}

	MenuItemType PopupMenu::getItemTypeAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::getItemTypeAt");
		return mItemsInfo[_index].type;
	}

	void PopupMenu::setItemNameAt(size_t _index, const Ogre::UTFString & _name)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::setItemNameAt");

		mItemsInfo[_index].name = _name;
		MenuItemPtr item = mItemsInfo[_index].item;
		item->setCaption(_name);
		/*IntSize size = item->getTextSize();
		size.width += 7; //FIXME
		mItemsInfo[_index].width = size.width;*/

		update();
	}

	void PopupMenu::setItemIdAt(size_t _index, const std::string & _id)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::setItemIdAt");
		mItemsInfo[_index].id = _id;
	}

	const std::string & PopupMenu::getItemIdAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::getItemIdAt");
		return mItemsInfo[_index].id;
	}

	void PopupMenu::_notifyDeleteItem(MenuItemPtr _item)
	{
		// общий шутдаун виджета
		if (mShutdown) return;

		size_t index = getItemIndex(_item);
		mItemsInfo.erase(mItemsInfo.begin() + index);
		update();
	}

	void PopupMenu::_notifyUpdateName(MenuItemPtr _item)
	{
		size_t index = getItemIndex(_item);
		mItemsInfo[index].width =
			_item->getTextSize().width + _item->getSize().width - _item->getTextCoord().width;
		update();
	}

} // namespace MyGUI
