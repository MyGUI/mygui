/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "MyGUI_ComboBox.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_List.h"
#include "MyGUI_Button.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	const float COMBO_ALPHA_MAX  = ALPHA_MAX;
	const float COMBO_ALPHA_MIN  = ALPHA_MIN;
	const float COMBO_ALPHA_COEF = 4.0f;

	ComboBox::ComboBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Edit(_coord, _align, _info, _parent, _creator, _name),
		mButton(null),
		mList(null),
		mListShow(false),
		mMaxHeight(0),
		mItemIndex(ITEM_NONE),
		mModeDrop(false),
		mDropMouse(false),
		mShowSmooth(false)
	{
		// парсим свойства
		const MapString & param = _info->getParams();
		MapString::const_iterator iter = param.find("HeightList");
		if (iter != param.end()) mMaxHeight = utility::parseInt(iter->second);

		iter = param.find("ListSmoothShow");
		if (iter != param.end()) setSmoothShow(utility::parseBool(iter->second));

		std::string listSkin, listLayer;
		iter = param.find("ListSkin");
		if (iter != param.end()) listSkin = iter->second;
		iter = param.find("ListLayer");
		if (iter != param.end()) listLayer = iter->second;

		// ручками создаем список
		mList = static_cast<ListPtr>(WidgetManager::getInstance().createWidget(List::_getType(), listSkin, IntCoord(), ALIGN_DEFAULT, null, this, ""));
		// присоединяем виджет с уровню и не добавляем себе
		LayerManager::getInstance().attachToLayerKeeper(listLayer, mList);

		//mList = static_cast<List>(_createWidget(List::_getType(), listSkin, IntCoord(), ALIGN_DEFAULT, listLayer, ""));
		mList->_setOwner(this);

		mList->hide();
		mList->eventKeyLostFocus = newDelegate(this, &ComboBox::notifyListLostFocus);
		mList->eventListSelectAccept = newDelegate(this, &ComboBox::notifyListSelectAccept);
		mList->eventListMouseItemActivate = newDelegate(this, &ComboBox::notifyListMouseItemActivate);
		mList->eventListChangePosition = newDelegate(this, &ComboBox::notifyListChangePosition);

		// парсим кнопку
		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Button") {
				mButton = castWidget<Button>(*iter);
				mButton->eventMouseButtonPressed = newDelegate(this, &ComboBox::notifyButtonPressed);
			}
		}
		MYGUI_ASSERT(null != mButton, "Child Button not found in skin (combobox must have Button)");

		// корректируем высоту списка
		if (mMaxHeight < (int)mList->getFontHeight()) mMaxHeight = (int)mList->getFontHeight();

		// подписываем дочерние классы на скролл
		mWidgetUpper->eventMouseWheel = newDelegate(this, &ComboBox::notifyMouseWheel);
		mWidgetUpper->eventMouseButtonPressed = newDelegate(this, &ComboBox::notifyMousePressed);

		//mWidgetCursor->eventMouseWheel = newDelegate(this, &ComboBox::notifyMouseWheel);
		//mWidgetCursor->eventMouseButtonPressed = newDelegate(this, &ComboBox::notifyMousePressed);

		// подписываемся на изменения текста
		eventEditTextChange = newDelegate(this, &ComboBox::notifyEditTextChange);
	}

	ComboBox::~ComboBox()
	{
		// чтобы теперь удалить, виджет должен быть в нашем списке
		mWidgetChild.push_back(mList);
		WidgetManager::getInstance().destroyWidget(mList);
	}

	void ComboBox::notifyButtonPressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (false == _left) return;

		mDropMouse = true;

		if (mListShow) hideList();
		else showList();
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

	void ComboBox::notifyListSelectAccept(MyGUI::WidgetPtr _widget, size_t _position)
	{
		std::string str;
		//size_t pos = mList->getItemSelect();
		if (_position != ITEM_NONE) {
			mItemIndex = _position;
			str = mList->getItem(_position);
		}
		Edit::setCaption(str);

		mDropMouse = false;

		InputManager::getInstance().setKeyFocusWidget(this);

		if (mModeDrop) eventComboAccept(this);
	}

	void ComboBox::notifyListChangePosition(MyGUI::WidgetPtr _widget, size_t _position)
	{
		eventComboChangePosition(this, _position);
	}

	void ComboBox::_onKeyButtonPressed(int _key, Char _char)
	{
		Edit::_onKeyButtonPressed(_key, _char);

		// при нажатии вниз, показываем лист
		if (_key == OIS::KC_DOWN) {
			// выкидываем список только если мыша свободна
			if (false == InputManager::getInstance().isCaptureMouse()) {
				showList();
			}
		}
		// нажат ввод в окне редиктирования
		else if (_key == OIS::KC_RETURN) {
			eventComboAccept(this);
		}

	}

	void ComboBox::notifyListMouseItemActivate(MyGUI::WidgetPtr _widget, size_t _position)
	{
		if (_position != ITEM_NONE) {
			mItemIndex = _position;
			Edit::setCaption(mList->getItem(_position));
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
				Edit::setCaption(mList->getItem(mItemIndex));
				mList->setItemSelect(mItemIndex);
				mList->beginToIndex(mItemIndex);
				eventComboChangePosition(this, mItemIndex);
			}
		}
		else if (_rel < 0) {
			if ((mItemIndex+1) < mList->getItemCount()) {
				if (mItemIndex == ITEM_NONE) mItemIndex = 0;
				else mItemIndex ++;
				Edit::setCaption(mList->getItem(mItemIndex));
				mList->setItemSelect(mItemIndex);
				mList->beginToIndex(mItemIndex);
				eventComboChangePosition(this, mItemIndex);
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
		if (ITEM_NONE != mItemIndex) {
			mItemIndex = ITEM_NONE;
			mList->setItemSelect(mItemIndex);
			mList->beginToStart();
			eventComboChangePosition(this, mItemIndex);
		}
	}

	void ComboBox::showList()
	{
		// пустой список не показываем
		if (mList->getItemCount() == 0) return;

		mListShow = true;

		int height = mList->getOptimalHeight();
		if (height > mMaxHeight) height = mMaxHeight;

		// берем глобальные координаты выджета
		IntCoord coord(WidgetManager::convertToGlobal(IntPoint(), this), mCoord.size());

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

		if (mShowSmooth) {
			ControllerFadeAlpha * controller = new ControllerFadeAlpha(COMBO_ALPHA_MAX, COMBO_ALPHA_COEF, true);
			ControllerManager::getInstance().addItem(mList, controller);
		}
		else mList->show();

		InputManager::getInstance().setKeyFocusWidget(mList);
	}

	void ComboBox::hideList()
	{
		mListShow = false;

		if (mShowSmooth) {
			ControllerFadeAlpha * controller = new ControllerFadeAlpha(COMBO_ALPHA_MIN, COMBO_ALPHA_COEF, false);
			controller->eventPostAction = newDelegate(action::actionWidgetHide);
			ControllerManager::getInstance().addItem(mList, controller);
		}
		else mList->hide();
	}

	void ComboBox::setItemSelect(size_t _index)
	{
		MYGUI_ASSERT(_index < mList->getItemCount(), "setComboItemIndex: index " << _index <<" out of range");
		mItemIndex = _index;
		Edit::setCaption(mList->getItem(_index));
		Edit::updateView(0); // hook for update
	}

	size_t ComboBox::getItemCount()
	{
		return mList->getItemCount();
	}

	const Ogre::DisplayString & ComboBox::getItem(size_t _index)
	{
		MYGUI_ASSERT(_index < mList->getItemCount(), "getItemString: index " << _index <<" out of range");
		return mList->getItem(_index);
	}

	void ComboBox::setItem(size_t _index, const Ogre::DisplayString & _item)
	{
		MYGUI_ASSERT(_index < mList->getItemCount(), "setItemString: index " << _index <<" out of range");
		mList->setItem(_index, _item);
		mItemIndex = ITEM_NONE;
		mList->setItemSelect(mItemIndex);
	}

	void ComboBox::insertItem(size_t _index, const Ogre::DisplayString & _item)
	{
		mList->insertItem(_index, _item);
		mItemIndex = ITEM_NONE;
		mList->setItemSelect(mItemIndex);
	}

	void ComboBox::deleteItem(size_t _index)
	{
		mList->deleteItem(_index);
		mItemIndex = ITEM_NONE;
		mList->setItemSelect(mItemIndex);
	}

	void ComboBox::addItem(const Ogre::DisplayString& _item)
	{
		mList->addItem(_item);
		mItemIndex = ITEM_NONE;
		mList->setItemSelect(mItemIndex);
	}

	void ComboBox::deleteAllItems()
	{
		mItemIndex = ITEM_NONE;
		mList->deleteAllItems();
	}

	void ComboBox::setComboModeDrop(bool _drop)
	{
		mModeDrop = _drop;
		setEditStatic(mModeDrop);
	}

} // namespace MyGUI
