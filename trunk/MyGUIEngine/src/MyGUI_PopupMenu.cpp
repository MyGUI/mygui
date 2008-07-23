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
		mMaxWidth(0)
	{
		// ��� ����� ����� �����
		mNeedKeyFocus = true;

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Client") {
				mWidgetClient = (*iter);
				//mWidgetClient->eventMouseButtonPressed = newDelegate(this, &List::notifyMousePressed);
			}
		}
		MYGUI_ASSERT(null != mWidgetClient, "Child Widget Client not found in skin (List must have Client)");

		// ������ ��������
		const MapString & param = _info->getParams();
		MapString::const_iterator iterS = param.find("SkinLine");
		if (iterS != param.end()) mSkinLine = iterS->second;
		MYGUI_ASSERT(false == mSkinLine.empty(), "SkinLine property or skin not found (PopupMenu must have SkinLine property)");

		iterS = param.find("HeightLine");
		if (iterS != param.end()) mHeightLine = utility::parseInt(iterS->second);
		if (mHeightLine < 1) mHeightLine = 1;

		// ������������� �������� ����
		hide();
	}

	void PopupMenu::insertItem(size_t _index, const Ogre::UTFString& _item, bool _submenu, bool _separator)
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

		mItems.insert(mItems.begin() + _index, ItemInfo(button, _separator, submenu));

		update();
	}

	void PopupMenu::setItem(size_t _index, const Ogre::UTFString& _item)
	{
		MYGUI_ASSERT(_index < mItems.size(), "index out of range");
		ButtonPtr button = mItems[_index].button;
		button->setCaption(_item);

		IntSize size = button->getTextSize();
		button->_setInternalData(size.width);

		update();
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
			if (iter->button->_getInternalData() > size.width) size.width = iter->button->_getInternalData();
		}

		setSize(size + mCoord.size() - mWidgetClient->getSize());
	}

	void PopupMenu::notifyMouseClick(MyGUI::WidgetPtr _sender)
	{
		// ����� ��������� �� �������� ����
		size_t index = ITEM_NONE;
		for (VectorPopupMenuItemInfo::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
			if (iter->button == _sender) {
				index = iter - mItems.begin();
				break;
			}
		}
		eventPopupMenuAccept(this, index);

		// ������ �������
		static_cast<ButtonPtr>(_sender)->setButtonPressed(true);

		// ���� ���� �������, �� ��������� ��� ����� �������, �� �������� ���� �� �����
		if (mItems[index].submenu == NULL)
		{
			hidePopupMenu();
		}
	}

	void PopupMenu::notifyOpenSubmenu(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		// ����� ��������� �� �������� ����
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
			mItems[index].submenu->showPopupMenu(IntPoint(getRight(), mItems[index].button->getTop() + getTop()));
		}
	}

	void PopupMenu::showPopupMenu(const IntPoint& _point, bool _checkBorders)
	{
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

	void PopupMenu::_onKeyLostFocus(WidgetPtr _new)
	{
		if (_new != null)
		{
			// �� ������� ���� ������� �������
			if (_new == this->_getOwner())
				return;
			// �� ������� ���� ������� ����
			WidgetPtr owner = _new->_getOwner();
			while (owner != null)
			{
				if (owner == this) return;
				owner = owner->_getOwner();
			}
		}
		hidePopupMenu();
		eventPopupMenuClose(this);

		// !!! ����������� �������� � ����� ������
		Widget::eventKeyLostFocus(mWidgetEventSender, _new);
	}

	void PopupMenu::hidePopupMenu(bool _hideParentPopup)
	{
		if ( _hideParentPopup && (NULL != _getOwner()) )
		{
			// ���� ��� ���� ����� ���� ��� ���� - ������� � ���
			if ( _getOwner()->getWidgetType() == getWidgetType() )
			{
				castWidget<PopupMenu>(_getOwner())->hidePopupMenu();
			}
			else if ( _getOwner()->getWidgetType() == MenuBar::_getType() )
			{
				castWidget<MenuBar>(_getOwner())->resetItemSelect();
			}
		}

		// ���������
		setEnabledSilent(false);
		// �������� ��������
		ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MIN, POPUP_MENU_SPEED_COEF, false);
		controller->eventPostAction = newDelegate(action::actionWidgetHide);
		ControllerManager::getInstance().addItem(this, controller);

		// ������ ���� �����
		for (VectorPopupMenuItemInfo::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter)
		{
			if (iter->submenu) {
				iter->submenu->hidePopupMenu(false);
			}
		}
	}

} // namespace MyGUI
