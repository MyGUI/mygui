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

	MYGUI_RTTI_CHILD_IMPLEMENT( ComboBox, Edit );

	const float COMBO_ALPHA_MAX  = ALPHA_MAX;
	const float COMBO_ALPHA_MIN  = ALPHA_MIN;
	const float COMBO_ALPHA_COEF = 4.0f;

	ComboBox::ComboBox(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name) :
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
		const MapString & properties = _info->getProperties();
		MapString::const_iterator iter = properties.find("HeightList");
		if (iter != properties.end()) mMaxHeight = utility::parseInt(iter->second);

		iter = properties.find("ListSmoothShow");
		if (iter != properties.end()) setSmoothShow(utility::parseBool(iter->second));

		std::string listSkin, listLayer;
		iter = properties.find("ListSkin");
		if (iter != properties.end()) listSkin = iter->second;
		iter = properties.find("ListLayer");
		if (iter != properties.end()) listLayer = iter->second;

		// ручками создаем список
		mList = static_cast<ListPtr>(WidgetManager::getInstance().createWidget(List::getClassTypeName(), listSkin, IntCoord(), Align::Default, null, this, ""));
		// присоединяем виджет с уровню и не добавляем себе
		LayerManager::getInstance().attachToLayerKeeper(listLayer, mList);

		//mList = static_cast<List>(_createWidget(List::_getType(), listSkin, IntCoord(), Align::Default, listLayer, ""));
		mList->_setOwner(this);

		mList->hide();
		mList->eventKeyLostFocus = newDelegate(this, &ComboBox::notifyListLostFocus);
		mList->eventListSelectAccept = newDelegate(this, &ComboBox::notifyListSelectAccept);
		mList->eventListMouseItemActivate = newDelegate(this, &ComboBox::notifyListMouseItemActivate);
		mList->eventListChangePosition = newDelegate(this, &ComboBox::notifyListChangePosition);

		// парсим кнопку
		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Button") {
				MYGUI_DEBUG_ASSERT( ! mButton, "widget already assigned");
				mButton = (*iter)->castType<Button>();
				mButton->eventMouseButtonPressed = newDelegate(this, &ComboBox::notifyButtonPressed);
			}
		}
		MYGUI_ASSERT(null != mButton, "Child Button not found in skin (combobox must have Button)");

		// корректируем высоту списка
		if (mMaxHeight < (int)mList->getFontHeight()) mMaxHeight = (int)mList->getFontHeight();

		// подписываем дочерние классы на скролл
		mWidgetClient->eventMouseWheel = newDelegate(this, &ComboBox::notifyMouseWheel);
		mWidgetClient->eventMouseButtonPressed = newDelegate(this, &ComboBox::notifyMousePressed);

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

	void ComboBox::notifyButtonPressed(WidgetPtr _sender, int _left, int _top, MouseButton _id)
	{
		if (MB_Left != _id) return;

		mDropMouse = true;

		if (mListShow) hideList();
		else showList();
	}

	void ComboBox::notifyListLostFocus(WidgetPtr _sender, WidgetPtr _new)
	{
		if (mDropMouse) {
			mDropMouse = false;
			WidgetPtr focus = InputManager::getInstance().getMouseFocusWidget();
			// кнопка сама уберет список
			if (focus == mButton) return;
			// в режиме дропа все окна учавствуют
			if ( (mModeDrop) && (focus == mWidgetClient) ) return;
		}

		hideList();
	}

	void ComboBox::notifyListSelectAccept(WidgetPtr _widget, size_t _position)
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

	void ComboBox::notifyListChangePosition(WidgetPtr _widget, size_t _position)
	{
		eventComboChangePosition(this, _position);
	}

	void ComboBox::_onKeyButtonPressed(KeyCode _key, Char _char)
	{
		Edit::_onKeyButtonPressed(_key, _char);

		// при нажатии вниз, показываем лист
		if (_key == KC_DOWN) {
			// выкидываем список только если мыша свободна
			if (false == InputManager::getInstance().isCaptureMouse()) {
				showList();
			}
		}
		// нажат ввод в окне редиктирования
		else if ((_key == KC_RETURN) || (_key == KC_NUMPADENTER)) {
			eventComboAccept(this);
		}

	}

	void ComboBox::notifyListMouseItemActivate(WidgetPtr _widget, size_t _position)
	{
		if (_position != ITEM_NONE) {
			mItemIndex = _position;
			Edit::setCaption(mList->getItem(_position));
		}

		InputManager::getInstance().setKeyFocusWidget(this);

		if (mModeDrop) eventComboAccept(this);
	}

	void ComboBox::notifyMouseWheel(WidgetPtr _sender, int _rel)
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

	void ComboBox::notifyMousePressed(WidgetPtr _sender, int _left, int _top, MouseButton _id)
	{
		// обязательно отдаем отцу, а то мы у него в наглую отняли
		Edit::notifyMousePressed(_sender, _left, _top, _id);

		mDropMouse = true;

		// показываем список
		if (mModeDrop) notifyButtonPressed(null, _left, _top, _id);
	}

	void ComboBox::notifyEditTextChange(WidgetPtr _sender)
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
		IntCoord coord = this->getAbsoluteCoord();

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
		MYGUI_ASSERT(_index < mList->getItemCount() || _index == ITEM_NONE, "setComboItemIndex: index " << _index <<" out of range");
		mItemIndex = _index;
		mList->setItemSelect(_index);
		if (_index == ITEM_NONE)
		{
			Edit::setCaption("");
			return;
		}
		Edit::setCaption(mList->getItem(_index));
		Edit::updateView(0); // hook for update
	}

	size_t ComboBox::getItemCount()
	{
		return mList->getItemCount();
	}

	const Ogre::UTFString & ComboBox::getItem(size_t _index)
	{
		MYGUI_ASSERT(_index < mList->getItemCount(), "getItem: index " << _index << " out of range");
		return mList->getItem(_index);
	}

	void ComboBox::setItem(size_t _index, const Ogre::UTFString & _item)
	{
		MYGUI_ASSERT(_index < mList->getItemCount(), "setItem: index " << _index << " out of range");
		mList->setItem(_index, _item);
		mItemIndex = ITEM_NONE;
		mList->setItemSelect(mItemIndex);
	}

	void ComboBox::setItemData(size_t _index, Any _data)
	{
		MYGUI_ASSERT(_index < mList->getItemCount(), "setItemData: index " << _index << " out of range");
		mList->setItemData(_index, _data);
		mItemIndex = ITEM_NONE;
		mList->setItemSelect(mItemIndex);
	}

	void ComboBox::insertItem(size_t _index, const Ogre::UTFString & _item, Any _data)
	{
		mList->insertItem(_index, _item, _data);
		mItemIndex = ITEM_NONE;
		mList->setItemSelect(mItemIndex);
	}

	void ComboBox::deleteItem(size_t _index)
	{
		mList->deleteItem(_index);
		mItemIndex = ITEM_NONE;
		mList->setItemSelect(mItemIndex);
	}

	void ComboBox::addItem(const Ogre::UTFString& _item, Any _data)
	{
		mList->addItem(_item, _data);
		mItemIndex = ITEM_NONE;
		mList->setItemSelect(mItemIndex);
	}

	void ComboBox::deleteAllItems()
	{
		mItemIndex = ITEM_NONE;
		mList->deleteAllItems();//FIXME заново созданные строки криво стоят
	}

	void ComboBox::setComboModeDrop(bool _drop)
	{
		mModeDrop = _drop;
		setEditStatic(mModeDrop);
	}

} // namespace MyGUI
