/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Common.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_Widget.h"
#include "MyGUI_RenderOut.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_PointerManager.h"

namespace MyGUI
{
	const std::string XML_TYPE("Lang");
	const std::string INPUT_DEFAULT_LANGUAGE("English");
	const unsigned long INPUT_TIME_DOUBLE_CLICK = 250; //measured in milliseconds
	const float INPUT_DELAY_FIRST_KEY = 0.4f;
	const float INPUT_INTERVAL_KEY = 0.05f;
	const size_t INPUT_COUNT_LOAD_CHAR = 116;
	const size_t INPUT_CHARSET_LIMIT = 65535;

	INSTANCE_IMPLEMENT(InputManager);

	void InputManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		mWidgetMouseFocus = 0;
		mWidgetKeyFocus = 0;
		mWidgetRootMouseFocus = 0;
		mWidgetRootKeyFocus = 0;
		mIsWidgetMouseCapture = false;
		mIsShiftPressed = false;
		mIsControlPressed = false;
		mHoldKey = KC_UNASSIGNED;
		//mUseOISKeyLayout = false;
		mFirstPressKey = true;
		mTimerKey = 0.0f;
		mOldAbsZ = 0;

		createDefaultCharSet();

		WidgetManager::getInstance().registerUnlinker(this);
		Gui::getInstance().addFrameListener(this);
		Gui::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &InputManager::_load);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void InputManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		Gui::getInstance().removeFrameListener(this);
		WidgetManager::getInstance().unregisterUnlinker(this);
		Gui::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool InputManager::injectMouseMove(int _absx, int _absy, int _absz)
	{
		// ���������� �������
		mMousePosition.set(_absx, _absy);

		// ��������� ������� �� ������
		int relz = _absz - mOldAbsZ;
		mOldAbsZ = _absz;

		// ������� ������
		PointerManager::getInstance().setPosition(mMousePosition);

		// �������� �� ������
		if (relz != 0) {
			if (mWidgetMouseFocus != null) mWidgetMouseFocus->_onMouseWheel(relz);
			return isFocusMouse();
		}

		if (mIsWidgetMouseCapture) {
			if (mWidgetMouseFocus != null) mWidgetMouseFocus->_onMouseDrag(_absx, _absy);
			else mIsWidgetMouseCapture = false;
			return true;
		}

		// ���� �������� ����
		LayerItem *  rootItem = null;
		WidgetPtr item = static_cast<WidgetPtr>(LayerManager::getInstance()._findLayerItem(_absx, _absy, rootItem));

		// ���������� �� ���������
		if (null != rootItem) {
			while (null != static_cast<WidgetPtr>(rootItem)->_getOwner()) {
				rootItem = static_cast<WidgetPtr>(rootItem)->_getOwner();
			}
		}	

		// ������ �� ����������
		if (mWidgetMouseFocus == item) {
			if (mWidgetMouseFocus != null) mWidgetMouseFocus->_onMouseMove(_absx, _absy);
			return isFocusMouse();
		}

		// ��������� �� �����������
		if (0 != mVectorModalRootWidget.size()) {
			if (rootItem != mVectorModalRootWidget.back()) {
				rootItem = null;
				item = null;
			}
		}

		// ����� ������, ��������� �� ����������� �������
		if ((mWidgetMouseFocus != null) && (mWidgetMouseFocus->isEnabled())) {
			mWidgetMouseFocus->_onMouseLostFocus(item);
		}

		if ((item != null) && (item->isEnabled())) {
			if (item->getPointer() != mPointer) {
				mPointer = item->getPointer();
				if (mPointer.empty()) PointerManager::getInstance().setDefaultPointer();
				else PointerManager::getInstance().setPointer(mPointer, item);

				eventChangeMousePointer(mPointer);
			}
			item->_onMouseMove(_absx, _absy);
			item->_onMouseSetFocus(mWidgetMouseFocus);

		}
		// ���������� ������
		else if (false == mPointer.empty()) {
			PointerManager::getInstance().setDefaultPointer();
			mPointer.clear();

			eventChangeMousePointer(mPointer);
		}

		// ��������� ������� �������
		if (rootItem != mWidgetRootMouseFocus) {
			if (mWidgetRootMouseFocus != null) mWidgetRootMouseFocus->_onMouseChangeRootFocus(false);
			if (rootItem != null) static_cast<WidgetPtr>(rootItem)->_onMouseChangeRootFocus(true);
			mWidgetRootMouseFocus = static_cast<WidgetPtr>(rootItem);
		}

		// ���������� ������� ����
		mWidgetMouseFocus = item;

		return isFocusMouse();
	}

	bool InputManager::injectMousePress(int _absx, int _absy, MouseButton _id)
	{

		// ���� �� �������� �� �� ���
		if (false == isFocusMouse()) {
			resetKeyFocusWidget();
			return false;
		}

		// ���� �������� ������� ������������
		if (false == mWidgetMouseFocus->isEnabled()) return true;

		// ����������� ������ �� ����� ������� � ������ ���� ������� ����
		if (MB_Left == _id) {
			// ������ ����
			mIsWidgetMouseCapture = true;
			// ���������� ����� �������
			mLastLeftPressed.set(_absx, _absy);
		}

		// ���� ����� ��� ������ ������� ����� ��������� �����
		WidgetPtr focus = mWidgetMouseFocus;
		while ((focus != null) && (false == focus->isNeedKeyFocus())) focus = focus->getParent();

		// ������������� ����� ������� �.�. �������� ������ ������� �������� ����� ��� ����
		setKeyFocusWidget(focus);

		if (mWidgetMouseFocus != null) {

			mWidgetMouseFocus->_onMouseButtonPressed(MB_Left == _id);
			//mWidgetMouseFocus->_onMouseButtonPressed(_id);

			// ��������� ������
			LayerManager::getInstance().upLayerItem(mWidgetMouseFocus);
		}
		return true;
	}

	bool InputManager::injectMouseRelease(int _absx, int _absy, MouseButton _id)
	{

		if (isFocusMouse() && mIsWidgetMouseCapture) {

			// ���� �������� ������� ������������
			if (false == mWidgetMouseFocus->isEnabled()) return true;

			// ���������� ������
			mIsWidgetMouseCapture = false;

			mWidgetMouseFocus->_onMouseButtonReleased(MB_Left == _id);
			//mWidgetMouseFocus->_onMouseButtonReleased(_id);

			// ����� ������, ������ ����� ���� ������
			if (null != mWidgetMouseFocus) {

				if ((MB_Left == _id) && mTime.getMilliseconds() < INPUT_TIME_DOUBLE_CLICK) {
					mWidgetMouseFocus->_onMouseButtonDoubleClick();
				}
				else {
					mTime.reset();
					// ��������� ��� ��� �� �� ����� ������ ��� � ���� ��� �������
					LayerItem * rootItem = null;
					WidgetPtr item = static_cast<WidgetPtr>(LayerManager::getInstance()._findLayerItem(_absx, _absy, rootItem));
					if ( item == mWidgetMouseFocus) {
						mWidgetMouseFocus->_onMouseButtonClick();
					}
				}
			}

			// ��� ����������� �����������
			injectMouseMove(_absx, _absy, mOldAbsZ);

			return true;
		}

		return false;
	}

	bool InputManager::injectKeyPress(KeyCode _key)
	{
		// �������� �� ������������ ������
		detectLangShift(_key, true);
		// ���������� �������
		storeKey(_key);

		//Pass keystrokes to the current active text widget
		if (isFocusKey()) {
			Char ch;
			/*if (mUseOISKeyLayout) {
				ch = _arg.text;
				if (std::find (mCurrentLanguage->second.begin(), mCurrentLanguage->second.end(), ch) == mCurrentLanguage->second.end())
					ch = 0;
			}
			else {*/
				ch = getKeyChar(_key);
			//}
			mWidgetKeyFocus->_onKeyButtonPressed(_key, ch);
		}

		return isFocusKey();
	}

	bool InputManager::injectKeyRelease(KeyCode _key)
	{
		// �������� �� ������������ ������
		detectLangShift(_key, false);
		// ���������� �������
		resetKey(_key);

		if (isFocusKey()) mWidgetKeyFocus->_onKeyButtonReleased(_key);

		return isFocusKey();
	}

    //Detects switching from an english to a other mode on a keyboard (?)
	void InputManager::detectLangShift(int keyEvent, bool bIsKeyPressed)
	{
		// ���� ����������� �� ����
		if (mMapLanguages.size() == 1) return;

		// ��� ���������� ������������� ������������ LeftAlt+LeftShift ��� LeftCtrl+LeftShift,LeftShift+LeftAlt ��� LeftShift+LeftCtrl
		static bool bIsFirstKeyPressed = false; // LeftAlt ��� LeftCtrl
		static bool bIsSecondKeyPressed = false; // LeftShift
		static bool bIsTwoKeyPressed = false; // ��� ���� ������

		if ((keyEvent == KC_LSHIFT) || (keyEvent == KC_RSHIFT)) {
			if (bIsKeyPressed) {
				mIsShiftPressed = true;
				bIsSecondKeyPressed = true;
				if (bIsFirstKeyPressed) bIsTwoKeyPressed = true;
			} else {
				mIsShiftPressed = false;
				bIsSecondKeyPressed = false;
				if ((!bIsFirstKeyPressed) && (bIsTwoKeyPressed)) {
					bIsTwoKeyPressed = false;
					// ��������� ����
					changeLanguage();
				}
			}
		}
		else if ((keyEvent == KC_LMENU) || (keyEvent == KC_RMENU)
			|| (keyEvent == KC_LCONTROL) || (keyEvent == KC_RCONTROL)) {

			if ((keyEvent == KC_LCONTROL) || (keyEvent == KC_RCONTROL)) mIsControlPressed = bIsKeyPressed;

			if (bIsKeyPressed) {
				bIsFirstKeyPressed = true;
				if (bIsSecondKeyPressed) bIsTwoKeyPressed = true;
			} else {
				bIsFirstKeyPressed = false;
				if ((!bIsSecondKeyPressed) && (bIsTwoKeyPressed)) {
					bIsTwoKeyPressed = false;
					// ��������� ����
					changeLanguage();
				}
			}
		}
		else {
			bIsFirstKeyPressed = false;
			bIsSecondKeyPressed = false;
			bIsTwoKeyPressed = false;
		}
	}

	Char InputManager::getKeyChar(int keyEvent) // ���������� ������ �� ��� ���� ����
	{
		if (keyEvent < 58) return mCurrentLanguage->second[keyEvent + (mIsShiftPressed ? 58 : 0)];
		else if (keyEvent < 84) {
			if (keyEvent > 70) return mNums[keyEvent-71];
		} else if (keyEvent == KC_DIVIDE) return mCurrentLanguage->second[KC_SLASH + (mIsShiftPressed ? 58 : 0)];
		else if (keyEvent == KC_OEM_102) return mCurrentLanguage->second[KC_BACKSLASH + (mIsShiftPressed ? 58 : 0)];
		return 0;
	}

	void InputManager::createDefaultCharSet()
	{
		// ��������� ���������� ����
		mMapLanguages[INPUT_DEFAULT_LANGUAGE] = LangInfo();
		mCurrentLanguage = mMapLanguages.find(INPUT_DEFAULT_LANGUAGE);
		mCurrentLanguage->second.resize(INPUT_COUNT_LOAD_CHAR);

		// ��������� ������
		Char chars[INPUT_COUNT_LOAD_CHAR] = {
			0, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 45, 61, 0, 0, 113, 119, 101, 114, 116, 121, 117, 105, 111, 112, 91, 93, 0, 0, 97, 115, 100, 102, 103, 104, 106, 107, 108, 59, 39, 96, 0, 92, 122, 120, 99, 118, 98, 110, 109, 44, 46, 47, 0, 42, 0, 32, // normal
			0, 0, 33, 64, 35, 36, 37, 94, 38, 42, 40, 41, 95, 43, 0, 0, 81, 87, 69, 82, 84, 89, 85, 73, 79, 80, 123, 125, 0, 0, 65, 83, 68, 70, 71, 72, 74, 75, 76, 58, 34, 126, 0, 124, 90, 88, 67, 86, 66, 78, 77, 60, 62, 63, 0, 42, 0, 32 // shift
		};
		// �������� � ���������� �����
		LangInfo & lang = mCurrentLanguage->second;
		for (size_t i=0; i<INPUT_COUNT_LOAD_CHAR; i++) lang[i] = chars[i];

		// ��������� ������� ��� �������
		Char nums[13] = {55, 56, 57, 45, 52, 53, 54, 43, 49, 50, 51, 48, 46};
		mNums.resize(13);
		// �������� � ���������� �����
		for (size_t i=0; i<13; i++) mNums[i] = nums[i];
	}

	bool InputManager::load(const std::string & _file, const std::string & _group)
	{
		return Gui::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void InputManager::_load(xml::xmlNodePtr _node, const std::string & _file)
	{
		xml::xmlNodeIterator lang = _node->getNodeIterator();
		while (lang.nextNode(XML_TYPE)) {

			std::string name;
			if ( false == lang->findAttribute("name", name)) continue;

			std::vector<std::string> chars = utility::split(lang->getBody());
			if (chars.size() == INPUT_COUNT_LOAD_CHAR) {

				// ������� ��������� ���� �� ����� ���� ���
				MapLang::iterator iter = mMapLanguages.find(name);
				MYGUI_ASSERT(iter == mMapLanguages.end(), "language '" << name << "' already exist");

				// ������� ����
				mMapLanguages[name] = LangInfo();
				iter = mMapLanguages.find(name);
				iter->second.resize(INPUT_COUNT_LOAD_CHAR);
				LangInfo & lang = iter->second;

				// � ��������� ���
				for (size_t j=0; j<INPUT_COUNT_LOAD_CHAR; j++) {
					unsigned int ch = utility::parseUInt(chars[j]);
					if (INPUT_CHARSET_LIMIT < ch) {
						lang[j] = 0;
						MYGUI_LOG(Warning, "character with code '" << ch << "' out of range");
					}
					else lang[j] = (Char)ch;
				}

			}
			else {
				MYGUI_LOG(Warning, "Quantity of characters is not " << INPUT_COUNT_LOAD_CHAR);
			}

		};

		// ����������� ��������� ��������, ��� ��� �� �������������� ��� ����������
		mCurrentLanguage = mMapLanguages.find(INPUT_DEFAULT_LANGUAGE);
	}

	void InputManager::setKeyFocusWidget(WidgetPtr _widget)
	{
		// ���� ������� �����
		WidgetPtr root = _widget;
		if (root != null) { while (root->_getOwner() != null) root = root->_getOwner(); }

		// ���� ������� ����� ���������, �� ���������
		if (mWidgetRootKeyFocus != root) {
			if (mWidgetRootKeyFocus != null) mWidgetRootKeyFocus->_onKeyChangeRootFocus(false);
			if (root != null) root->_onKeyChangeRootFocus(true);
			mWidgetRootKeyFocus = root;
		}

		// � ��� ��� ��� ��������� ������������ �����
		if (_widget == mWidgetKeyFocus) return;

		if (isFocusKey()) mWidgetKeyFocus->_onKeyLostFocus(_widget);
		if (_widget != null) {
			if (_widget->isNeedKeyFocus()) {
				_widget->_onKeySetFocus(mWidgetKeyFocus);
				mWidgetKeyFocus = _widget;
				return;
			}
		}
		mWidgetKeyFocus = null;

	}

	void InputManager::resetMouseFocusWidget()
	{
		mIsWidgetMouseCapture = false;
		if (null != mWidgetMouseFocus) {
			mWidgetMouseFocus->_onMouseLostFocus(null);
			mWidgetMouseFocus = null;
		}
		if (null != mWidgetRootMouseFocus) {
			mWidgetRootMouseFocus->_onMouseChangeRootFocus(false);
			mWidgetRootMouseFocus = null;
		}
	}

	void InputManager::_frameEntered(float _frame)
	{
		if ( mHoldKey == KC_UNASSIGNED) return;
		if ( false == isFocusKey() ) {
			mHoldKey = KC_UNASSIGNED;
			//mFirstPressKey = true;
			//mTimerKey = 0.0f;
			return;
		}

		mTimerKey += _frame;

		if (mFirstPressKey) {
			if (mTimerKey > INPUT_DELAY_FIRST_KEY) {
				mFirstPressKey = false;
				mTimerKey = 0.0f;
			}
		} else {
			if (mTimerKey > INPUT_INTERVAL_KEY) {
				mTimerKey -= INPUT_INTERVAL_KEY;
				mWidgetKeyFocus->_onKeyButtonPressed(mHoldKey, getKeyChar(mHoldKey));
				// focus can be dropped in _onKeyButtonPressed
				if ( isFocusKey() ) mWidgetKeyFocus->_onKeyButtonReleased(mHoldKey);
			}
		}

	}

	// ������� ������ ������ �� ���� ��������� ����
	void InputManager::_unlinkWidget(WidgetPtr _widget)
	{
		if (null == _widget) return;
		if (_widget == mWidgetMouseFocus) {
			mIsWidgetMouseCapture = false;
			mWidgetMouseFocus = null;
		}
		if (_widget == mWidgetKeyFocus) mWidgetKeyFocus = null;
		if (_widget == mWidgetRootMouseFocus) mWidgetRootMouseFocus = null;
		if (_widget == mWidgetRootKeyFocus) mWidgetRootKeyFocus = null;

		// ������� ����������, ����� �� ������ ������
		for (VectorWidgetPtr::iterator iter=mVectorModalRootWidget.begin(); iter!=mVectorModalRootWidget.end(); ++iter) {
			if ((*iter == _widget)) {
				mVectorModalRootWidget.erase(iter);
				break;
			}
		}

	}

	void InputManager::addWidgetModal(WidgetPtr _widget)
	{
		if (null == _widget) return;
		MYGUI_ASSERT(null == _widget->getParent(), "Modal widget must be root");

		resetMouseFocusWidget();
		removeWidgetModal(_widget);
		mVectorModalRootWidget.push_back(_widget);

		setKeyFocusWidget(_widget);
		LayerManager::getInstance().upLayerItem(_widget);
	}

	void InputManager::removeWidgetModal(WidgetPtr _widget)
	{
		resetKeyFocusWidget(_widget);
		resetMouseFocusWidget();

		for (VectorWidgetPtr::iterator iter=mVectorModalRootWidget.begin(); iter!=mVectorModalRootWidget.end(); ++iter) {
			if ((*iter == _widget)) {
				mVectorModalRootWidget.erase(iter);
				break;
			}
		}
		// ���� ��� ���� ��������� �� �� ���������� � ���������
		if (false == mVectorModalRootWidget.empty()) {
			setKeyFocusWidget(mVectorModalRootWidget.back());
			LayerManager::getInstance().upLayerItem(mVectorModalRootWidget.back());
		}
	}

	void InputManager::storeKey(int _key)
	{
		mHoldKey = KC_UNASSIGNED;

		if ( false == isFocusKey() ) return;
		if ( (_key == KC_LSHIFT) || (_key == KC_RSHIFT)
			|| (_key == KC_LCONTROL) || (_key == KC_RCONTROL)
			|| (_key == KC_LMENU) || (_key == KC_RMENU)
			) return;

		mFirstPressKey = true;
		mHoldKey = _key;
		mTimerKey = 0.0f;
	}

	void InputManager::resetKey(int _key)
	{
		mHoldKey = KC_UNASSIGNED;
	}

	WidgetPtr InputManager::getWidgetFromPoint(int _left, int _top)
	{
		LayerItem * root = null;
		return static_cast<WidgetPtr>(LayerManager::getInstance()._findLayerItem(_left, _top, root));
	}

} // namespace MyGUI
