/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_PopupMenu.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_Button.h"
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
		mSubmenuImageSize(0)
	{
		// ��� ����� ����� �����
		mNeedKeyFocus = true;

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Client") {
				MYGUI_DEBUG_ASSERT( ! mWidgetClient, "widget already assigned");
				mWidgetClient = (*iter);
				//mWidgetClient->eventMouseButtonPressed = newDelegate(this, &List::notifyMousePressed);
			}
		}
		MYGUI_ASSERT(null != mWidgetClient, "Child Widget Client not found in skin (List must have Client)");
		mWidgetClient->eventMouseButtonReleased = newDelegate(this, &PopupMenu::notifyMouseReleased);

		// ������ ��������
		const MapString & properties = _info->getProperties();
		MapString::const_iterator iterS = properties.find("SkinLine");
		if (iterS != properties.end()) mSkinLine = iterS->second;
		MYGUI_ASSERT(false == mSkinLine.empty(), "SkinLine property not found (PopupMenu must have SkinLine property)");

		iterS = properties.find("HeightLine");
		if (iterS != properties.end()) mHeightLine = utility::parseInt(iterS->second);
		if (mHeightLine < 1){
			MYGUI_LOG(Warning, "PopupMenu HeightLine can't be less thah 1. Set to 1.");
			mHeightLine = 1;
		}

		iterS = properties.find("SubmenuImageSize");
		if (iterS != properties.end()) mSubmenuImageSize = utility::parseInt(iterS->second);

		iterS = properties.find("SubMenuSkin");
		if (iterS != properties.end()) mSubMenuSkin = iterS->second;
		MYGUI_ASSERT(false == mSubMenuSkin.empty(), "SubMenuSkin property not found (PopupMenu must have SubMenuSkin property)");

		iterS = properties.find("SubMenuLayer");
		if (iterS != properties.end()) mSubMenuLayer = iterS->second;
		MYGUI_ASSERT(false == mSubMenuLayer.empty(), "SubMenuLayer property not found (PopupMenu must have SubMenuLayer property)");

		// ������������� �������� ����
		hide();
	}

	void PopupMenu::setButtonImageIndex(ButtonPtr _button, size_t _index)
	{
		StaticImagePtr image = _button->getStaticImage();
		if ( null == image ) return;
		if (image->getItemResource()) {
			static const int CountIcons = 2;
			static const char * IconNames[CountIcons + 1] = {"None", "Popup", ""};
			if (_index >= CountIcons) _index = CountIcons;
			image->setItemName(IconNames[_index]);
		}
		else {
			image->setItemSelect(_index);
		}
	}

	void PopupMenu::insertItemAt(size_t _index, const Ogre::UTFString & _item, ItemType _type, Any _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItemsInfo.size(), "PopupMenu::insertItem");
		if (_index == ITEM_NONE) _index = mItemsInfo.size();

		ButtonPtr button = mWidgetClient->createWidget<Button>(mSkinLine, IntCoord(0, 0, mWidgetClient->getWidth(), mHeightLine), Align::Top | Align::HStretch);
		button->setCaption(_item);
		button->eventMouseButtonClick = newDelegate(this, &PopupMenu::notifyMouseClick);
		button->eventMouseMove = newDelegate(this, &PopupMenu::notifyOpenSubmenu);
		button->eventMouseButtonReleased = newDelegate(this, &PopupMenu::notifyMouseReleased);

		setButtonImageIndex(button, _type == ItemTypePopup ? ItemImagePopup : ItemImageNone);
		IntSize size = button->getTextSize();
		size.width += 7; //FIXME
		button->_setInternalData(size.width);

		PopupMenuPtr submenu = null;
		if (_type == ItemTypePopup)
		{
			submenu = Gui::getInstance().createWidget<PopupMenu>(mSubMenuSkin, IntCoord(), Align::Default, mSubMenuLayer);
			submenu->_setOwner(this);
		}

		mItemsInfo.insert(mItemsInfo.begin() + _index, ItemInfo(button, _type == ItemTypeSeparator, submenu, _data));

		update();
	}

	/*void PopupMenu::replaceItemAt(size_t _index, const Ogre::UTFString & _item, ItemType _type)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::replaceItem");

		ButtonPtr button = mItemsInfo[_index].button;
		button->setCaption(_item);
		IntSize size = button->getTextSize();
		size.width += 7; //FIXME
		button->_setInternalData(size.width);

		mItemsInfo[_index].separator = _type == ItemTypeSeparator;

		// ��������� ������ �����
		if (_type == ItemTypePopup) {
			if (mItemsInfo[_index].submenu == null) {
				mItemsInfo[_index].submenu = Gui::getInstance().createWidget<PopupMenu>(mSubMenuSkin, IntCoord(), Align::Default, mSubMenuLayer);
				mItemsInfo[_index].submenu->_setOwner(this);
			}
			else {
				mItemsInfo[_index].submenu->removeAllItems();
			}
		}
		else if (mItemsInfo[_index].submenu != null) {
			WidgetManager::getInstance().destroyWidget(mItemsInfo[_index].submenu);
			mItemsInfo[_index].submenu = null;
		}

		update();
	}*/

	void PopupMenu::removeItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::removeItemAt");

		WidgetManager::getInstance().destroyWidget(mItemsInfo[_index].button);
		if ( mItemsInfo[_index].submenu )
			WidgetManager::getInstance().destroyWidget(mItemsInfo[_index].submenu);
		mItemsInfo.erase(mItemsInfo.begin() + _index);

		update();
	}

	void PopupMenu::removeAllItems()
	{
		if (false == mItemsInfo.empty()) {
			WidgetManager & manager = WidgetManager::getInstance();
			for (VectorPopupMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter) {
				manager.destroyWidget(iter->button);
				if ( iter->submenu )
					manager.destroyWidget(iter->submenu);
			}
			mItemsInfo.clear();

			update();
		}
	}

	const Ogre::UTFString& PopupMenu::getItemNameAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::getItemNameAt");
		return mItemsInfo[_index].button->getCaption();
	}

	void PopupMenu::update()
	{
		IntSize size;
		for (VectorPopupMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter) {
			iter->button->setPosition(0, size.height);
			size.height += mHeightLine;
			if (iter->separator) size.height += 10;

			int width = iter->button->_getInternalData();
			if (iter->submenu != null) width += mSubmenuImageSize;
			if (width > size.width) size.width = width;
		}

		setSize(size + mCoord.size() - mWidgetClient->getSize());
	}

	bool PopupMenu::isRelative(WidgetPtr _widget, bool _all)
	{
		if (_widget != null)
		{
			// ��, ������
			if (_widget == this->_getOwner())
				return true;
			// ����, ���� � ������ �������
			WidgetPtr owner = _widget->_getOwner();
			while (owner != null)
			{
				if (owner == this) return true;
				owner = owner->_getOwner();
			}
			if (_all)
			{
				// ��� ��� � �!
				if (_widget == this) return true;
				 // ������
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
		// ����� ��������� �� �������� ����
		size_t index = ITEM_NONE;
		for (VectorPopupMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter) {
			if (iter->button == _sender) {
				index = iter - mItemsInfo.begin();
				break;
			}
		}
		// ���� ������� ����� ������� onMouseRelease � �� ������
		if (index == ITEM_NONE) return;
		eventPopupMenuAccept(this, index);

		// ������ �������
		static_cast<ButtonPtr>(_sender)->setButtonPressed(true);

		// ���� ���� �������, �� ��������� ��� ����� �������, �� �������� ���� �� �����
		if (mItemsInfo[index].submenu == null)
		{
			hidePopupMenu();
		}
	}

	void PopupMenu::notifyOpenSubmenu(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		// ����� ��������� �� �������� ����
		size_t index = ITEM_NONE;
		for (VectorPopupMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter)
		{
			if (iter->button == _sender) {
				index = iter - mItemsInfo.begin();
				break;
			}
		}
		for (VectorPopupMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter)
		{
			if (iter->submenu)
			{
				iter->button->setButtonPressed(false);
				iter->submenu->hidePopupMenu(false);
			}
		}
		if (mItemsInfo[index].submenu)
		{
			mItemsInfo[index].button->setButtonPressed(true);
			// ��������� ���� �������
			IntPoint position(getRight(), mItemsInfo[index].button->getTop() + getTop());
			if (position.left + mItemsInfo[index].submenu->getWidth() > MyGUI::Gui::getInstance().getViewWidth())
				position.left -= mItemsInfo[index].submenu->getWidth() + getWidth();
			if (position.top + mItemsInfo[index].submenu->getHeight() > MyGUI::Gui::getInstance().getViewHeight())
				position.top = mItemsInfo[index].button->getBottom() - mItemsInfo[index].submenu->getHeight() + getTop();
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

		for (VectorPopupMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter) {
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
		// ���� ������ �� �� ��� �� ������
		if ( _sender->getAbsoluteCoord().inside(IntPoint(_left, _top)) == false )
		{
			MyGUI::LayerItem * rootItem = null;
			MyGUI::WidgetPtr item = static_cast<MyGUI::WidgetPtr>(LayerManager::getInstance()._findLayerItem(_left, _top, rootItem));
			MyGUI::WidgetPtr button = item; // ����� �����������, ��� ������ notifyMouseClick
			// ��������� ������ ������� �������, ����� �� ��������� ����� ������
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

	void PopupMenu::_onKeyLostFocus(WidgetPtr _new)
	{
		// �� �������, ���� ����� ������� � ������� ��� ����
		if (isRelative(_new)) return;
		hidePopupMenu();
		eventPopupMenuClose(this);

		// !!! ����������� �������� � ����� ������
		Widget::eventKeyLostFocus(mWidgetEventSender, _new);
	}

	void PopupMenu::hidePopupMenu(bool _hideParentPopup)
	{
		if ( _hideParentPopup && mOwner != null )
		{
			// ���� ��� ���� ����� ���� ��� ���� - ������� � ���
			PopupMenuPtr popup = mOwner->castType<PopupMenu>(false);
			if (popup != null) popup->hidePopupMenu();
			else {
				MenuBarPtr menu = mOwner->castType<MenuBar>(false);
				if (menu != null) menu->clearItemSelected();
			}
		}

		// ���������
		setEnabledSilent(false);
		// �������� ��������
		ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MIN, POPUP_MENU_SPEED_COEF, false);
		controller->eventPostAction = newDelegate(action::actionWidgetHide);
		ControllerManager::getInstance().addItem(this, controller);

		// ������ ���� �����
		for (VectorPopupMenuItemInfo::iterator iter=mItemsInfo.begin(); iter!=mItemsInfo.end(); ++iter)
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

	PopupMenu::ItemType PopupMenu::getItemTypeAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::getItemTypeAt");
		return getItemType(mItemsInfo[_index].submenu != null, mItemsInfo[_index].separator);
	}

	void PopupMenu::replaceItemNameAt(size_t _index, const Ogre::UTFString & _name)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::replaceItemNameAt");

		ButtonPtr button = mItemsInfo[_index].button;
		button->setCaption(_name);
		IntSize size = button->getTextSize();
		size.width += 7; //FIXME
		button->_setInternalData(size.width);

		update();
	}

} // namespace MyGUI
