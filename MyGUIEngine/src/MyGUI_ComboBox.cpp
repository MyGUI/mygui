/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "MyGUI_ComboBox.h"
#include "MyGUI_InputManager.h"

namespace MyGUI
{

	ComboBox::ComboBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Edit(_coord, _align, _info, _parent, _name),
		mListShow(false)
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

		mList->eventListSelectAccept = newDelegate(this, &ComboBox::notifyListSelectAccept);
		mList->eventListChangePosition = newDelegate(this, &ComboBox::notifyListChangePosition);
		mList->eventListMouseChangePosition = newDelegate(this, &ComboBox::notifyListMouseChangePosition);

		mList->addItemString("Line 1");
		mList->addItemString("Line 2");
		mList->addItemString("Line 3");
		mList->addItemString("Line 4");

	}

	void ComboBox::notifyButtonPressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (false == _left) return;

		if (mListShow) hideList();
		else showList();
	}

	void ComboBox::showList()
	{
		mListShow = true;
		IntCoord coord = mCoord;
		coord.top += coord.height;
		coord.height = 100;
		mList->setPosition(coord);
		mList->show();

		InputManager::getInstance().setKeyFocusWidget(mList);
	}

	void ComboBox::hideList()
	{
		mListShow = false;
		mList->hide();
		mList->setItemSelect(ITEM_NONE);
	}

	void ComboBox::notifyListLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
	{
		// кнопка сама уберет список
		if (InputManager::getInstance().getMouseFocusWidget() == mButton) return;

		hideList();
	}

	void ComboBox::notifyListSelectAccept(MyGUI::WidgetPtr _widget)
	{
		std::string str;
		size_t pos = mList->getItemSelect();
		if (pos != ITEM_NONE) str = mList->getItemString(pos);
		setCaption(str);

		InputManager::getInstance().setKeyFocusWidget(this);

//		mList->setItemSelect(ITEM_NONE);
	}

	void ComboBox::_onKeyButtonPressed(int _key, wchar_t _char)
	{
		Edit::_onKeyButtonPressed(_key, _char);
		// при нажатии вниз, показываем лист
		if (_key == OIS::KC_DOWN) showList();
	}

	void ComboBox::notifyListChangePosition(MyGUI::WidgetPtr _widget, size_t _position)
	{
		//if (_position != ITEM_NONE) setCaption(mList->getItemString(_position));
		//else setCaption("");
	}

	void ComboBox::notifyListMouseChangePosition(MyGUI::WidgetPtr _widget, size_t _position)
	{
		if (_position != ITEM_NONE) setCaption(mList->getItemString(_position));
		//else setCaption("");

		InputManager::getInstance().setKeyFocusWidget(this);
	}

} // namespace MyGUI