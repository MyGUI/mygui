/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_PopupMenu.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_Button.h"
#include "MyGUI_WidgetManager.h"

#include "MyGUI_ControllerManager.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_InputManager.h"

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
		// нам нужен фокус клавы
		mNeedKeyFocus = true;

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Client") {
				mWidgetClient = (*iter);
				//mWidgetClient->eventMouseButtonPressed = newDelegate(this, &List::notifyMousePressed);
			}
		}
		MYGUI_ASSERT(null != mWidgetClient, "Child Widget Client not found in skin (List must have Client)");

		// парсим свойства
		const MapString & param = _info->getParams();
		MapString::const_iterator iterS = param.find("SkinLine");
		if (iterS != param.end()) mSkinLine = iterS->second;
		MYGUI_ASSERT(false == mSkinLine.empty(), "SkinLine property or skin not found (PopupMenu must have SkinLine property)");

		iterS = param.find("HeightLine");
		if (iterS != param.end()) mHeightLine = utility::parseInt(iterS->second);
		if (mHeightLine < 1) mHeightLine = 1;

		// первоначально скрываем окно
		hide();

	}

	void PopupMenu::insertItem(size_t _index, const Ogre::UTFString& _item, bool _separator)
	{
		if (_index > m_listWidgets.size()) _index = m_listWidgets.size();
		WidgetPtr widget = mWidgetClient->createWidget<Button>(mSkinLine, IntCoord(0, 0, mWidgetClient->getWidth(), mHeightLine), ALIGN_TOP | ALIGN_HSTRETCH);
		widget->eventMouseButtonClick = newDelegate(this, &PopupMenu::notifyMouseClick);
		widget->setCaption(_item);

		IntSize size = widget->getTextSize();
		size.width += 7;
		widget->_setInternalData(size.width);

		m_listWidgets.insert(m_listWidgets.begin() + _index, widget);
		m_listSeparators.insert(m_listSeparators.begin() + _index, _separator);

		update();
	}

	void PopupMenu::setItem(size_t _index, const Ogre::UTFString& _item)
	{
		MYGUI_ASSERT(_index < m_listWidgets.size(), "index out of range");
		WidgetPtr widget = m_listWidgets[_index];
		widget->setCaption(_item);

		IntSize size = widget->getTextSize();
		widget->_setInternalData(size.width);

		update();
	}

	void PopupMenu::deleteItem(size_t _index)
	{
		MYGUI_ASSERT(_index < m_listWidgets.size(), "index out of range");
		WidgetManager::getInstance().destroyWidget(m_listWidgets[_index]);
		m_listWidgets.erase(m_listWidgets.begin() + _index);

		update();
	}

	void PopupMenu::deleteAllItems()
	{
		if (false == m_listWidgets.empty()) {
			WidgetManager & manager = WidgetManager::getInstance();
			for (VectorWidgetPtr::iterator iter=m_listWidgets.begin(); iter!=m_listWidgets.end(); ++iter) {
				manager.destroyWidget(*iter);
			}
			m_listWidgets.clear();

			update();
		}
	}

	size_t PopupMenu::getItemCount()
	{
		return m_listWidgets.size();
	}

	const Ogre::UTFString& PopupMenu::getItem(size_t _index)
	{
		MYGUI_ASSERT(_index < m_listWidgets.size(), "index out of range");
		return m_listWidgets[_index]->getCaption();
	}

	void PopupMenu::update()
	{
		IntSize size;
		for (VectorWidgetPtr::iterator iter=m_listWidgets.begin(); iter!=m_listWidgets.end(); ++iter) {
			(*iter)->setPosition(0, size.height);
			size.height += mHeightLine;
			if (m_listSeparators[iter - m_listWidgets.begin()]) size.height += 10;
			if ((*iter)->_getInternalData() > size.width) size.width = (*iter)->_getInternalData();
		}

		setSize(size + mCoord.size() - mWidgetClient->getSize());
	}

	void PopupMenu::notifyMouseClick(MyGUI::WidgetPtr _sender)
	{
		// потом передалть на интернал дата
		size_t index = ITEM_NONE;
		for (VectorWidgetPtr::iterator iter=m_listWidgets.begin(); iter!=m_listWidgets.end(); ++iter) {
			if ((*iter) == _sender) {
				index = iter - m_listWidgets.begin();
				break;
			}
		}
		eventPopupMenuAccept(this, index);

		// блокируем
		setEnabledSilent(false);

		// делаем нажатой
		static_cast<ButtonPtr>(_sender)->setButtonPressed(true);

		hidePopupMenu();

		//ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MIN, POPUP_MENU_SPEED_COEF, false);
		//controller->eventPostAction = newDelegate(action::actionWidgetHide);
		//ControllerManager::getInstance().addItem(this, controller);
	}

	void PopupMenu::showPopupMenu(const IntPoint& _point)
	{
		setPosition(_point);
		InputManager::getInstance().setKeyFocusWidget(this);

		for (VectorWidgetPtr::iterator iter=m_listWidgets.begin(); iter!=m_listWidgets.end(); ++iter) {
			if (static_cast<ButtonPtr>(*iter)->getButtonPressed()) {
				static_cast<ButtonPtr>(*iter)->setButtonPressed(false);
			}
		}

		ControllerManager::getInstance().removeItem(this);

		ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MAX, POPUP_MENU_SPEED_COEF, true);
		ControllerManager::getInstance().addItem(this, controller);
	}

	/*void PopupMenu::_onKeyChangeRootFocus(bool _focus)
	{
		if (false == _focus) {
			hidePopupMenu();
			eventPopupMenuClose(this);
		}

		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onKeyChangeRootFocus(_focus);
	}*/

	void PopupMenu::_onKeyLostFocus(WidgetPtr _new)
	{
		if (_new != null)
		{
			while (_new->_getOwner() != null)
			{
				if (_new == this) return;
				_new = _new->_getOwner();
			}
		}
		hidePopupMenu();
		eventPopupMenuClose(this);

		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::eventKeyLostFocus(mWidgetEventSender, _new);
	}

	void PopupMenu::hidePopupMenu()
	{
		// блокируем
		setEnabledSilent(false);
		// медленно скрываем
		ControllerFadeAlpha * controller = new ControllerFadeAlpha(ALPHA_MIN, POPUP_MENU_SPEED_COEF, false);
		controller->eventPostAction = newDelegate(action::actionWidgetHide);
		ControllerManager::getInstance().addItem(this, controller);
	}

} // namespace MyGUI
