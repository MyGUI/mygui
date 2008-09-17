/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Common.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_Widget.h"
#include "MyGUI_RenderOut.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_Gui.h"

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

#if MYGUI_DEBUG_MODE == 1
		m_showFocus = false;
#endif

		createDefaultCharSet();

		WidgetManager::getInstance().registerUnlinker(this);
		Gui::getInstance().eventFrameStart += newDelegate(this, &InputManager::frameEntered);
		//Gui::getInstance().addFrameListener(newDelegate(this, &InputManager::frameEntered), null);
		ResourceManager::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &InputManager::_load);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void InputManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		Gui::getInstance().eventFrameStart -= newDelegate(this, &InputManager::frameEntered);
		//Gui::getInstance().removeFrameListener(newDelegate(this, &InputManager::frameEntered));
		WidgetManager::getInstance().unregisterUnlinker(this);
		ResourceManager::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

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
			bool isFocus = isFocusMouse();
			if (mWidgetMouseFocus != null) mWidgetMouseFocus->_onMouseWheel(relz);
			return isFocus;
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
			bool isFocus = isFocusMouse();
			if (mWidgetMouseFocus != null) mWidgetMouseFocus->_onMouseMove(_absx, _absy);
			return isFocus;
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
				if (mPointer.empty()) {
					PointerManager::getInstance().setDefaultPointer();
					eventChangeMousePointer(PointerManager::getInstance().getDefaultPointer());
				}
				else {
					PointerManager::getInstance().setPointer(mPointer, item);
					eventChangeMousePointer(mPointer);
				}
			}
			item->_onMouseMove(_absx, _absy);
			item->_onMouseSetFocus(mWidgetMouseFocus);

		}
		// ���������� ������
		else if (false == mPointer.empty()) {
			PointerManager::getInstance().setDefaultPointer();
			mPointer.clear();

			eventChangeMousePointer(PointerManager::getInstance().getDefaultPointer());
		}

		// ��������� ������� �������
		if (rootItem != mWidgetRootMouseFocus) {
			if (mWidgetRootMouseFocus != null) mWidgetRootMouseFocus->_onMouseChangeRootFocus(false);
			if (rootItem != null) static_cast<WidgetPtr>(rootItem)->_onMouseChangeRootFocus(true);
			mWidgetRootMouseFocus = static_cast<WidgetPtr>(rootItem);
		}

		// ���������� ������� ����
		mWidgetMouseFocus = item;

#if MYGUI_DEBUG_MODE == 1
		if (m_showFocus) updateFocusWidgetHelpers();
#endif

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

			mWidgetMouseFocus->_onMouseButtonPressed(_absx, _absy, _id);
			//mWidgetMouseFocus->_onMouseButtonPressed(_id);

			// ��������� ������
			LayerManager::getInstance().upLayerItem(mWidgetMouseFocus);
		}
		return true;
	}

	bool InputManager::injectMouseRelease(int _absx, int _absy, MouseButton _id)
	{

		if (isFocusMouse()) {

			// ���� �������� ������� ������������
			if (false == mWidgetMouseFocus->isEnabled()) return true;

			mWidgetMouseFocus->_onMouseButtonReleased(_absx, _absy, _id);

			if (mIsWidgetMouseCapture) {

				// ���������� ������
				mIsWidgetMouseCapture = false;

				// ����� ������, ������ ����� ���� ������
				if (null != mWidgetMouseFocus) {

					if ((MB_Left == _id) && mTime.getMilliseconds() < INPUT_TIME_DOUBLE_CLICK) {
						mWidgetMouseFocus->_onMouseButtonDoubleClick();
					}
					else {
						// ��������� ��� ��� �� �� ����� ������ ��� � ���� ��� �������
						LayerItem * rootItem = null;
						WidgetPtr item = static_cast<WidgetPtr>(LayerManager::getInstance()._findLayerItem(_absx, _absy, rootItem));
						if ( item == mWidgetMouseFocus) {
							mWidgetMouseFocus->_onMouseButtonClick();
						}
						mTime.reset();
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

		bool wasFocusKey = isFocusKey();

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

		return wasFocusKey;
	}

	bool InputManager::injectKeyRelease(KeyCode _key)
	{
		// �������� �� ������������ ������
		detectLangShift(_key, false);
		// ���������� �������
		resetKey(_key);

		bool wasFocusKey = isFocusKey();

		if (isFocusKey()) mWidgetKeyFocus->_onKeyButtonReleased(_key);

		return wasFocusKey;
	}

    //Detects switching from an english to a other mode on a keyboard (?)
	void InputManager::detectLangShift(KeyCode keyEvent, bool bIsKeyPressed)
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

	Char InputManager::getKeyChar(KeyCode keyEvent) // ���������� ������ �� ��� ���� ����
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
		return ResourceManager::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
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
		if (_widget != mWidgetKeyFocus) {
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

#if MYGUI_DEBUG_MODE == 1
		if (m_showFocus) updateFocusWidgetHelpers();
#endif
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
#if MYGUI_DEBUG_MODE == 1
		if (m_showFocus) updateFocusWidgetHelpers();
#endif
	}

	// ������� ������ ������ �� ���� ��������� ����
	void InputManager::_unlinkWidget(WidgetPtr _widget)
	{
		if (null == _widget) return;
		if (_widget == mWidgetMouseFocus) {
			mIsWidgetMouseCapture = false;
			mWidgetMouseFocus = null;
#if MYGUI_DEBUG_MODE == 1
		if (m_showFocus) updateFocusWidgetHelpers();
#endif
		}
		if (_widget == mWidgetKeyFocus) {
			mWidgetKeyFocus = null;
#if MYGUI_DEBUG_MODE == 1
		if (m_showFocus) updateFocusWidgetHelpers();
#endif
		}
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

	void InputManager::storeKey(KeyCode _key)
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

	void InputManager::resetKey(KeyCode _key)
	{
		mHoldKey = KC_UNASSIGNED;
	}

	WidgetPtr InputManager::getWidgetFromPoint(int _left, int _top)
	{
		LayerItem * root = null;
		return static_cast<WidgetPtr>(LayerManager::getInstance()._findLayerItem(_left, _top, root));
	}

	void InputManager::frameEntered(float _frame)
	{
#if MYGUI_DEBUG_MODE == 1
		if (m_showFocus) updateFocusWidgetHelpers();
#endif
		if ( mHoldKey == KC_UNASSIGNED) return;
		if ( false == isFocusKey() ) {
			mHoldKey = KC_UNASSIGNED;
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
				while (mTimerKey > INPUT_INTERVAL_KEY) mTimerKey -= INPUT_INTERVAL_KEY;
				mWidgetKeyFocus->_onKeyButtonPressed(mHoldKey, getKeyChar(mHoldKey));
				// focus can be dropped in _onKeyButtonPressed
				if ( isFocusKey() ) mWidgetKeyFocus->_onKeyButtonReleased(mHoldKey);
			}
		}

	}

	void InputManager::setCurrentLanguage(const std::string & _lang)
	{
		MapLang::iterator iter = mMapLanguages.find(_lang);
		if (iter != mMapLanguages.end()) {
			mCurrentLanguage = iter;
		}
		else {
			MYGUI_LOG(Warning, "language '" << _lang << "' not found");
		}
	}

#if MYGUI_DEBUG_MODE == 1
	void InputManager::updateFocusWidgetHelpers()
	{

		const std::string layer = "Statistic";
		static WidgetPtr mouse_focus = null;
		static WidgetPtr mouse_helper = null;
		if ((mWidgetMouseFocus != mouse_focus) || ((mWidgetMouseFocus != null) && (mouse_helper != null) && mWidgetMouseFocus->getAbsoluteCoord() != mouse_helper->getAbsoluteCoord())) {
			mouse_focus = mWidgetMouseFocus;

			if (mouse_helper == null) {
				if (!LayerManager::getInstance().isExist(layer)) return;
				mouse_helper = Gui::getInstance().createWidget<Widget>("DebugMarkerGreen", IntCoord(), Align::Default, layer);
				mouse_helper->setNeedMouseFocus(false);
			}

			if (mWidgetMouseFocus) {
				MYGUI_OUT("mouse focus : ", mWidgetMouseFocus->getName());
				mouse_helper->setPosition(mWidgetMouseFocus->getAbsoluteCoord());
				mouse_helper->show();
			}
			else {
				MYGUI_OUT("mouse focus : null");
				mouse_helper->hide();
			}
		}

		static WidgetPtr key_focus = null;
		static WidgetPtr key_helper = null;
		if ((mWidgetKeyFocus != key_focus) || ((mWidgetKeyFocus != null) && (key_helper != null) && mWidgetKeyFocus->getAbsoluteCoord() != key_helper->getAbsoluteCoord())) {
			key_focus = mWidgetKeyFocus;

			if (key_helper == null) {
				if (!LayerManager::getInstance().isExist(layer)) return;
				key_helper = Gui::getInstance().createWidget<Widget>("DebugMarkerRed", IntCoord(), Align::Default, layer);
				key_helper->setNeedMouseFocus(false);
			}
			if (mWidgetKeyFocus) {
				MYGUI_OUT("key focus : ", mWidgetKeyFocus->getName());
				key_helper->setPosition(mWidgetKeyFocus->getAbsoluteCoord());
				key_helper->show();
			}
			else {
				MYGUI_OUT("key focus : null");
				key_helper->hide();
			}
		}
	}
#endif

} // namespace MyGUI
