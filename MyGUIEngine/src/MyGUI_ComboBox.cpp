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
		mModeDrop(false)
	{
		// запомием размер скина
		IntSize size = _info->getSize();

		// парсим свойства
		const MapString & param = _info->getParams();

		// парсим конечную кнопку
		mButton = parseSubWidget(param, "Button", "SkinDrop", "OffsetDrop", "AlignDrop", size);
		MYGUI_ASSERT(null != mButton);
		mButton->eventMouseButtonPressed = newDelegate(this, &ComboBox::notifyButtonPressed);

		// создаем выпадающий список
		MapString::const_iterator iter = param.find("LayerList");
		MYGUI_ASSERT(iter != param.end());
		mList = castWidget<List>(parseSubWidget(param, "List", "SkinList", "OffsetList", "AlignList", size, iter->second));
		mList->hide();
		mList->eventKeyLostFocus = newDelegate(this, &ComboBox::notifyListLostFocus);

		iter = param.find("HeightList");
		if (iter != param.end()) mMaxHeight = util::parseInt(iter->second);
		if (mMaxHeight < (int)mList->getFontHeight()) mMaxHeight = (int)mList->getFontHeight();

		mList->eventListSelectAccept = newDelegate(this, &ComboBox::notifyListSelectAccept);
		mList->eventListChangePosition = newDelegate(this, &ComboBox::notifyListChangePosition);
		mList->eventListMouseChangePosition = newDelegate(this, &ComboBox::notifyListMouseChangePosition);

		// подписываем дочерние классы на скролл
		mWidgetUpper->eventMouseWheel = newDelegate(this, &ComboBox::notifyMouseWheel);
		mWidgetUpper->eventMouseButtonPressed = newDelegate(this, &ComboBox::notifyMousePressed);

		mWidgetCursor->eventMouseWheel = newDelegate(this, &ComboBox::notifyMouseWheel);
		mWidgetCursor->eventMouseButtonPressed = newDelegate(this, &ComboBox::notifyMousePressed);


		mList->addItemString("Line 1");
		mList->addItemString("Line 2");
		mList->addItemString("Line 3");
		mList->addItemString("Line 4");
		mList->addItemString("Line 5");
		mList->addItemString("Line 6");
		mList->addItemString("Line 7");
		mList->addItemString("Line 8");
		mList->addItemString("Line 9");
		mList->addItemString("Line 10");
		mList->addItemString("Line 11");//*/

	}

	void ComboBox::notifyButtonPressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (false == _left) return;

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
		//mList->setItemSelect(ITEM_NONE);
	}

	void ComboBox::notifyListLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
	{
		WidgetPtr focus = InputManager::getInstance().getMouseFocusWidget();
		// кнопка сама уберет список
		if (focus == mButton) return;
		// в режиме дропа все окна учавствуют
		if ( (mModeDrop) && (focus == mWidgetUpper) ) return;

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
		setCaption(str);

		InputManager::getInstance().setKeyFocusWidget(this);

		if (mModeDrop) eventComboAccept(this);

//		mList->setItemSelect(ITEM_NONE);
	}

	void ComboBox::_onKeyButtonPressed(int _key, wchar_t _char)
	{
		Edit::_onKeyButtonPressed(_key, _char);

		// при нажатии вниз, показываем лист
		if (_key == OIS::KC_DOWN) showList();
		// нажат ввод в окне редиктирования
		else if (_key == OIS::KC_RETURN) {
			eventComboAccept(this);
		}

	}

	void ComboBox::notifyListChangePosition(MyGUI::WidgetPtr _widget, size_t _position)
	{
		//if (_position != ITEM_NONE) setCaption(mList->getItemString(_position));
		//else setCaption("");
	}

	void ComboBox::notifyListMouseChangePosition(MyGUI::WidgetPtr _widget, size_t _position)
	{
		if (_position != ITEM_NONE) {
			mItemIndex = _position;
			setCaption(mList->getItemString(_position));
		}
		//else setCaption("");

		InputManager::getInstance().setKeyFocusWidget(this);

		if (mModeDrop) eventComboAccept(this);
	}

	void ComboBox::notifyMouseWheel(MyGUI::WidgetPtr _sender, int _rel)
	{
		if (mList->getItemCount() == 0) return;
		if (InputManager::getInstance().getKeyFocusWidget() != this) return;

		if (_rel > 0) {
			if (mItemIndex != 0) {
				if (mItemIndex == ITEM_NONE) mItemIndex = 0;
				else mItemIndex --;
				setCaption(mList->getItemString(mItemIndex));
				mList->setItemSelect(mItemIndex);
				mList->beginToIndex(mItemIndex);
			}
		}
		else if (_rel < 0) {
			if ((mItemIndex+1) < mList->getItemCount()) {
				if (mItemIndex == ITEM_NONE) mItemIndex = 0;
				else mItemIndex ++;
				setCaption(mList->getItemString(mItemIndex));
				mList->setItemSelect(mItemIndex);
				mList->beginToIndex(mItemIndex);
			}
		}
	}

	void ComboBox::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		// обязательно отдаем отцу, а то мы у него в наглую отняли
		Edit::notifyMousePressed(_sender, _left);

		// показываем список
		if (mModeDrop) notifyButtonPressed(null, _left);
	}

} // namespace MyGUI