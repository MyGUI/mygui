/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "MyGUI_ComboBox.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_ControllerFadeAlpha.h"

namespace MyGUI
{

	const float COMBO_ALPHA_MAX  = ALPHA_MAX;
	const float COMBO_ALPHA_MIN  = ALPHA_MIN;
	const float COMBO_ALPHA_COEF = 4.0f;

	ComboBox::ComboBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Edit(_coord, _align, _info, _parent, _name),
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
		const MapString & param = _info->getParams();
		MapString::const_iterator iter = param.find("HeightList");
		if (iter != param.end()) mMaxHeight = util::parseInt(iter->second);

		iter = param.find("ListSmoothShow");
		if (iter != param.end()) setSmoothShow(util::parseBool(iter->second));

		std::string listSkin, listLayer;
		iter = param.find("ListSkin");
		if (iter != param.end()) listSkin = iter->second;
		iter = param.find("ListLayer");
		if (iter != param.end()) listLayer = iter->second;

		// ������� ������
		mList = Gui::getInstance().createWidget<List>(listSkin, IntCoord(), ALIGN_DEFAULT, listLayer);
		mList->_setOwner(this);
		mList->hide();
		mList->eventKeyLostFocus = newDelegate(this, &ComboBox::notifyListLostFocus);
		mList->eventListSelectAccept = newDelegate(this, &ComboBox::notifyListSelectAccept);
		mList->eventListMouseItemActivate = newDelegate(this, &ComboBox::notifyListMouseItemActivate);
		mList->eventListChangePosition = newDelegate(this, &ComboBox::notifyListChangePosition);

		// ������ ������
		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->getInternalString() == "Button") {
				mButton = castWidget<Button>(*iter);
				mButton->eventMouseButtonPressed = newDelegate(this, &ComboBox::notifyButtonPressed);
			}
		}
		MYGUI_ASSERT(null != mButton, "Child Button not found in skin (combobox must have Button)");

		// ������������ ������ ������
		if (mMaxHeight < (int)mList->getFontHeight()) mMaxHeight = (int)mList->getFontHeight();

		// ����������� �������� ������ �� ������
		mWidgetUpper->eventMouseWheel = newDelegate(this, &ComboBox::notifyMouseWheel);
		mWidgetUpper->eventMouseButtonPressed = newDelegate(this, &ComboBox::notifyMousePressed);

		mWidgetCursor->eventMouseWheel = newDelegate(this, &ComboBox::notifyMouseWheel);
		mWidgetCursor->eventMouseButtonPressed = newDelegate(this, &ComboBox::notifyMousePressed);

		// ������������� �� ��������� ������
		eventEditTextChange = newDelegate(this, &ComboBox::notifyEditTextChange);
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
			// ������ ���� ������ ������
			if (focus == mButton) return;
			// � ������ ����� ��� ���� ����������
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

	void ComboBox::notifyListChangePosition(MyGUI::WidgetPtr _widget, size_t _position)
	{
		eventComboChangePosition(this, _position);
	}

	void ComboBox::_onKeyButtonPressed(int _key, wchar_t _char)
	{
		Edit::_onKeyButtonPressed(_key, _char);

		// ��� ������� ����, ���������� ����
		if (_key == OIS::KC_DOWN) {
			// ���������� ������ ������ ���� ���� ��������
			if (false == InputManager::getInstance().isCaptureMouse()) {
				showList();
			}
		}
		// ����� ���� � ���� ��������������
		else if (_key == OIS::KC_RETURN) {
			eventComboAccept(this);
		}

	}

	void ComboBox::notifyListMouseItemActivate(MyGUI::WidgetPtr _widget, size_t _position)
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
				eventComboChangePosition(this, mItemIndex);
			}
		}
		else if (_rel < 0) {
			if ((mItemIndex+1) < mList->getItemCount()) {
				if (mItemIndex == ITEM_NONE) mItemIndex = 0;
				else mItemIndex ++;
				Edit::setCaption(mList->getItemString(mItemIndex));
				mList->setItemSelect(mItemIndex);
				mList->beginToIndex(mItemIndex);
				eventComboChangePosition(this, mItemIndex);
			}
		}
	}

	void ComboBox::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		// ����������� ������ ����, � �� �� � ���� � ������ ������
		Edit::notifyMousePressed(_sender, _left);

		mDropMouse = true;

		// ���������� ������
		if (mModeDrop) notifyButtonPressed(null, _left);
	}

	void ComboBox::notifyEditTextChange(MyGUI::WidgetPtr _sender)
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

		int height = mList->getListMaxHeight();
		if (height > mMaxHeight) height = mMaxHeight;

		// ����� ���������� ���������� �������
		IntCoord coord(WidgetManager::convertToGlobal(IntPoint(), this), mCoord.size());

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
			ControllerManager::getInstance().addItem(
				mList, new ControllerFadeAlpha(COMBO_ALPHA_MAX, COMBO_ALPHA_COEF, ControllerFadeAlpha::ACTION_NONE, true));
		}
		else mList->show();

		InputManager::getInstance().setKeyFocusWidget(mList);
	}

	void ComboBox::hideList()
	{
		mListShow = false;

		if (mShowSmooth) {
			ControllerManager::getInstance().addItem(
				mList, new ControllerFadeAlpha(COMBO_ALPHA_MIN, COMBO_ALPHA_COEF, ControllerFadeAlpha::ACTION_HIDE, false));
		}
		else mList->hide();
	}

} // namespace MyGUI
