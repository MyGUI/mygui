/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "MyGUI_ComboBox.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	ComboBox::ComboBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Edit(_coord, _align, _info, _parent, _name),
		mListShow(false),
		mMaxHeight(0),
		mItemIndex(ITEM_NONE),
		mModeDrop(false),
		mDropMouse(false)
	{
		// запомием размер скина
		IntSize size = _info->getSize();

		// парсим свойства
		const MapString & param = _info->getParams();

		MapString::const_iterator iter = param.find("ComboModeDropList");
		if (iter != param.end()) setComboModeDrop(util::parseBool(iter->second));

		// парсим конечную кнопку
		mButton = parseSubWidget(param, "Button", "SkinDrop", "OffsetDrop", "AlignDrop", size);
		MYGUI_ASSERT(null != mButton);
		mButton->eventMouseButtonPressed = newDelegate(this, &ComboBox::notifyButtonPressed);

		// создаем выпадающий список
		iter = param.find("LayerList");
		MYGUI_ASSERT(iter != param.end());
		mList = castWidget<List>(parseSubWidget(param, "List", "SkinList", "OffsetList", "AlignList", size, iter->second));
		mList->hide();
		mList->eventKeyLostFocus = newDelegate(this, &ComboBox::notifyListLostFocus);

		iter = param.find("HeightList");
		if (iter != param.end()) mMaxHeight = util::parseInt(iter->second);
		if (mMaxHeight < (int)mList->getFontHeight()) mMaxHeight = (int)mList->getFontHeight();

		mList->eventListSelectAccept = newDelegate(this, &ComboBox::notifyListSelectAccept);
		mList->eventListMouseChangePosition = newDelegate(this, &ComboBox::notifyListMouseChangePosition);

		// подписываем дочерние классы на скролл
		mWidgetUpper->eventMouseWheel = newDelegate(this, &ComboBox::notifyMouseWheel);
		mWidgetUpper->eventMouseButtonPressed = newDelegate(this, &ComboBox::notifyMousePressed);

		mWidgetCursor->eventMouseWheel = newDelegate(this, &ComboBox::notifyMouseWheel);
		mWidgetCursor->eventMouseButtonPressed = newDelegate(this, &ComboBox::notifyMousePressed);


		// подписываемся на изменения текста
		eventEditTextChange = newDelegate(this, &ComboBox::notifyEditTextChange);

	}

	void ComboBox::notifyButtonPressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (false == _left) return;

		mDropMouse = true;

		if (mListShow) hideList();
		else showList();
	}

	void ComboBox::showList()
	{
		// пустой списое не показываем
		if (mList->getItemCount() == 0) return;

		mListShow = true;

		int height = mList->getListMaxHeight();
		if (height > mMaxHeight) height = mMaxHeight;


		IntCoord coord = mCoord;

		//показываем список вверх
		if ((coord.top + coord.height + height) > (int)Gui::getInstance().getViewHeight()) {
			coord.height = height;
			coord.top -= coord.height;
		}
		// показываем список вниз
		else {
			coord.top += coord.height;
			coord.height = height;
		}
		mList->setPosition(coord);
		mList->show();

		InputManager::getInstance().setKeyFocusWidget(mList);
	}

	void ComboBox::hideList()
	{
		mListShow = false;
		mList->hide();
	}

	void ComboBox::notifyListLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
	{
		if (mDropMouse) {
			mDropMouse = false;
			WidgetPtr focus = InputManager::getInstance().getMouseFocusWidget();
			// кнопка сама уберет список
			if (focus == mButton) return;
			// в режиме дропа все окна учавствуют
			if ( (mModeDrop) && (focus == mWidgetUpper) ) return;
		}

		hideList();
	}

	void ComboBox::notifyListSelectAccept(MyGUI::WidgetPtr _widget)
	{
		std::string str;
		size_t pos = mList->getItemSelect();
		if (pos != ITEM_NONE) {
			mItemIndex = pos;
			str = mList->getItemString(pos);
		}
		Edit::setCaption(str);

		mDropMouse = false;

		InputManager::getInstance().setKeyFocusWidget(this);

		if (mModeDrop) eventComboAccept(this);
	}

	void ComboBox::_onKeyButtonPressed(int _key, wchar_t _char)
	{
		Edit::_onKeyButtonPressed(_key, _char);

		// при нажатии вниз, показываем лист
		if (_key == OIS::KC_DOWN) {

			// выкидываем список только если мыша свободна
			if (false == InputManager::getInstance().isCaptureMouse()) {
				showList();
			}
			//InputManager::getInstance().resetMouseFocusWidget();
		}
		// нажат ввод в окне редиктирования
		else if (_key == OIS::KC_RETURN) {
			eventComboAccept(this);
		}

	}

	void ComboBox::notifyListMouseChangePosition(MyGUI::WidgetPtr _widget, size_t _position)
	{
		if (_position != ITEM_NONE) {
			mItemIndex = _position;
			Edit::setCaption(mList->getItemString(_position));
		}

		InputManager::getInstance().setKeyFocusWidget(this);

		if (mModeDrop) eventComboAccept(this);
	}

	void ComboBox::notifyMouseWheel(MyGUI::WidgetPtr _sender, int _rel)
	{
		if (mList->getItemCount() == 0) return;
		if (InputManager::getInstance().getKeyFocusWidget() != this) return;
		if (InputManager::getInstance().isCaptureMouse()) return;

		if (_rel > 0) {
			if (mItemIndex != 0) {
				if (mItemIndex == ITEM_NONE) mItemIndex = 0;
				else mItemIndex --;
				Edit::setCaption(mList->getItemString(mItemIndex));
				mList->setItemSelect(mItemIndex);
				mList->beginToIndex(mItemIndex);
			}
		}
		else if (_rel < 0) {
			if ((mItemIndex+1) < mList->getItemCount()) {
				if (mItemIndex == ITEM_NONE) mItemIndex = 0;
				else mItemIndex ++;
				Edit::setCaption(mList->getItemString(mItemIndex));
				mList->setItemSelect(mItemIndex);
				mList->beginToIndex(mItemIndex);
			}
		}
	}

	void ComboBox::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		// обязательно отдаем отцу, а то мы у него в наглую отняли
		Edit::notifyMousePressed(_sender, _left);

		mDropMouse = true;

		// показываем список
		if (mModeDrop) notifyButtonPressed(null, _left);
	}

	void ComboBox::notifyEditTextChange(MyGUI::WidgetPtr _sender)
	{
		// сбрасываем выделенный элемент
		mItemIndex = ITEM_NONE;
		mList->setItemSelect(mItemIndex);
		mList->beginToStart();
	}

} // namespace MyGUI