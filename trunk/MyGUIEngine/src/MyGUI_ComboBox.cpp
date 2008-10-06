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
		// ������ ��������
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

		// ������� ������� ������
		mList = static_cast<ListPtr>(WidgetManager::getInstance().createWidget(List::getClassTypeName(), listSkin, IntCoord(), Align::Default, null, this, ""));
		// ������������ ������ � ������ � �� ��������� ����
		LayerManager::getInstance().attachToLayerKeeper(listLayer, mList);

		//mList = static_cast<List>(_createWidget(List::_getType(), listSkin, IntCoord(), Align::Default, listLayer, ""));
		mList->_setOwner(this);

		mList->hide();
		mList->eventKeyLostFocus = newDelegate(this, &ComboBox::notifyListLostFocus);
		mList->eventListSelectAccept = newDelegate(this, &ComboBox::notifyListSelectAccept);
		mList->eventListMouseItemActivate = newDelegate(this, &ComboBox::notifyListMouseItemActivate);
		mList->eventListChangePosition = newDelegate(this, &ComboBox::notifyListChangePosition);

		// ������ ������
		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Button") {
				MYGUI_DEBUG_ASSERT( ! mButton, "widget already assigned");
				mButton = (*iter)->castType<Button>();
				mButton->eventMouseButtonPressed = newDelegate(this, &ComboBox::notifyButtonPressed);
			}
		}
		MYGUI_ASSERT(null != mButton, "Child Button not found in skin (combobox must have Button)");

		// ������������ ������ ������
		if (mMaxHeight < (int)mList->getFontHeight()) mMaxHeight = (int)mList->getFontHeight();

		// ����������� �������� ������ �� ������
		mWidgetClient->eventMouseWheel = newDelegate(this, &ComboBox::notifyMouseWheel);
		mWidgetClient->eventMouseButtonPressed = newDelegate(this, &ComboBox::notifyMousePressed);

		//mWidgetCursor->eventMouseWheel = newDelegate(this, &ComboBox::notifyMouseWheel);
		//mWidgetCursor->eventMouseButtonPressed = newDelegate(this, &ComboBox::notifyMousePressed);

		// ������������� �� ��������� ������
		eventEditTextChange = newDelegate(this, &ComboBox::notifyEditTextChange);
	}

	ComboBox::~ComboBox()
	{
		// ����� ������ �������, ������ ������ ���� � ����� ������
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
			// ������ ���� ������ ������
			if (focus == mButton) return;
			// � ������ ����� ��� ���� ����������
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

		// ��� ������� ����, ���������� ����
		if (_key == KC_DOWN) {
			// ���������� ������ ������ ���� ���� ��������
			if (false == InputManager::getInstance().isCaptureMouse()) {
				showList();
			}
		}
		// ����� ���� � ���� ��������������
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
		// ����������� ������ ����, � �� �� � ���� � ������ ������
		Edit::notifyMousePressed(_sender, _left, _top, _id);

		mDropMouse = true;

		// ���������� ������
		if (mModeDrop) notifyButtonPressed(null, _left, _top, _id);
	}

	void ComboBox::notifyEditTextChange(WidgetPtr _sender)
	{
		// ���������� ���������� �������
		if (ITEM_NONE != mItemIndex) {
			mItemIndex = ITEM_NONE;
			mList->setItemSelect(mItemIndex);
			mList->beginToStart();
			eventComboChangePosition(this, mItemIndex);
		}
	}

	void ComboBox::showList()
	{
		// ������ ������ �� ����������
		if (mList->getItemCount() == 0) return;

		mListShow = true;

		int height = mList->getOptimalHeight();
		if (height > mMaxHeight) height = mMaxHeight;

		// ����� ���������� ���������� �������
		IntCoord coord = this->getAbsoluteCoord();

		//���������� ������ �����
		if ((coord.top + coord.height + height) > (int)Gui::getInstance().getViewHeight()) {
			coord.height = height;
			coord.top -= coord.height;
		}
		// ���������� ������ ����
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
		mList->deleteAllItems();//FIXME ������ ��������� ������ ����� �����
	}

	void ComboBox::setComboModeDrop(bool _drop)
	{
		mModeDrop = _drop;
		setEditStatic(mModeDrop);
	}

} // namespace MyGUI
