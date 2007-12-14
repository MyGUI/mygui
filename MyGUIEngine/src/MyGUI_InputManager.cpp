/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "xmlDocument.h"
#include "MyGUI_Common.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	const std::string INPUT_DEFAULT_LANGUAGE = "English";
	const int INPUT_TIME_DOUBLE_CLICK = 250; //measured in milliseconds
	const float INPUT_DELAY_FIRST_KEY = 0.4f;
	const float INPUT_INTERVAL_KEY = 0.05f;

	INSTANCE_IMPLEMENT(InputManager);

	void InputManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise);
		MYGUI_LOG("* Initialise: ", INSTANCE_TYPE_NAME);

		mWidgetMouseFocus = 0;
		mWidgetKeyFocus = 0;
		mWidgetRootMouseFocus = 0;
		mWidgetRootKeyFocus = 0;
		mIsWidgetMouseCapture = false;
		mIsCharShift = false;
		mHoldKey = OIS::KC_UNASSIGNED;

		createDefaultCharSet();

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully initialized");
		mIsInitialise = true;
	}

	void InputManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG("* Shutdown: ", INSTANCE_TYPE_NAME);

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully shutdown");
		mIsInitialise = false;
	}

	bool InputManager::injectMouseMove( const OIS::MouseEvent & _arg)
	{
		// ���������� �������
		mMousePosition.set(_arg.state.X.abs, _arg.state.Y.abs);

		// ������� ������
		PointerManager::getInstance().setPosition(IntPoint(_arg.state.X.abs, _arg.state.Y.abs));

		// �������� �� ������
		if (_arg.state.Z.rel != 0) {
			if (mWidgetMouseFocus != null) mWidgetMouseFocus->_onMouseSheel(_arg.state.Z.rel);
			return isCaptureMouse();
		}

		if (mIsWidgetMouseCapture) {
			if (mWidgetMouseFocus != null) mWidgetMouseFocus->_onMouseMove(_arg.state.X.abs, _arg.state.Y.abs);
			else mIsWidgetMouseCapture = false;
			return true;
		}

		// ���� �������� ����
		LayerItemInfoPtr rootItem = null;
		WidgetPtr item = static_cast<WidgetPtr>(LayerManager::getInstance().findWidgetItem(_arg.state.X.abs, _arg.state.Y.abs, rootItem));

		// ������ �� ����������
		if (mWidgetMouseFocus == item) return isCaptureMouse();

		// ����� ������, ��������� �� ����������� �������
		if ((mWidgetMouseFocus != null) && (mWidgetMouseFocus->isEnabled())) mWidgetMouseFocus->_onMouseLostFocus(item);
		if ((item != null) && (item->isEnabled())) item->_onMouseSetFocus(mWidgetMouseFocus);

		// ��������� ������� �������
		if (rootItem != mWidgetRootMouseFocus) {
			if (mWidgetRootMouseFocus != null) mWidgetRootMouseFocus->_onMouseChangeRootFocus(false);
			if (rootItem != null) static_cast<WidgetPtr>(rootItem)->_onMouseChangeRootFocus(true);
			mWidgetRootMouseFocus = static_cast<WidgetPtr>(rootItem);
		}

		// ���������� ������� ����
		mWidgetMouseFocus = item;

		return isCaptureMouse();
	}

	bool InputManager::injectMousePress( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id )
	{

		// ���� �� �������� ��  �� ���
		if (false == isCaptureMouse()) {
			resetKeyFocusWidget();
			return false;
		}

		// ���� �������� ������� ������������
		if (false == mWidgetMouseFocus->isEnabled()) return true;

		// ����������� ������ �� ����� ������� � ������ ���� ������� ����
		if ( _id == OIS::MB_Left ) {
			// ������ ����
			mIsWidgetMouseCapture = true;
			// ���������� ����� �������
			mLastLeftPressed.set((int)_arg.state.X.abs, (int)_arg.state.Y.abs);
		}
			
		// ������������� ����� ������� �.�. �������� ������ ������� �������� ����� ��� ����
		setKeyFocusWidget(mWidgetMouseFocus);
		if (mWidgetMouseFocus != null) mWidgetMouseFocus->_onMouseButtonPressed(_id == OIS::MB_Left);

		// ��������� ������, ��������
		if (mWidgetMouseFocus != null) {
			WidgetPtr tmp = mWidgetMouseFocus;
			while (tmp->getParent() != null) tmp = tmp->getParent();
			LayerManager::getInstance().upItem(tmp);
		}
		return true;
	}

	bool InputManager::injectMouseRelease( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id )
	{

		if (isCaptureMouse() && mIsWidgetMouseCapture) {

			// ���� �������� ������� ������������
			if (false == mWidgetMouseFocus->isEnabled()) return true;

			// ���������� ������
			mIsWidgetMouseCapture = false;

			mWidgetMouseFocus->_onMouseButtonReleased(_id == OIS::MB_Left);

			// ����� ������, ������ ����� ���� ������
			if (null != mWidgetMouseFocus) {

				if ((_id == OIS::MB_Left) && mTime.getMilliseconds() < INPUT_TIME_DOUBLE_CLICK) {
					mWidgetMouseFocus->_onMouseButtonClick(true);
					//OUT("double click");
				}
				else {
					mTime.reset();
					// ��������� ��� ��� �� �� ����� ������ ��� � ���� ��� �������
					LayerItemInfoPtr rootItem = null;
					WidgetPtr item = static_cast<WidgetPtr>(LayerManager::getInstance().findWidgetItem(_arg.state.X.abs, _arg.state.Y.abs, rootItem));
					if ( item == mWidgetMouseFocus) {
						mWidgetMouseFocus->_onMouseButtonClick(false);
					}
				}
			}

			// ��� ����������� �����������
			injectMouseMove(_arg);

			return true;
		}

		return false;
	}

	bool InputManager::injectKeyPress(const OIS::KeyEvent & _arg)
	{
		// �������� �� ������������ ������
		detectLangShift(_arg.key, true);
		
		//Pass keystrokes to the current active text widget
		if (isCaptureKey()) mWidgetKeyFocus->_onKeyButtonPressed(_arg.key, getKeyChar(_arg.key));

		// ���������� �������
		storeKey(_arg.key);

		return isCaptureKey();
	}

	bool InputManager::injectKeyRelease(const OIS::KeyEvent & _arg)
	{
		// ���������� �������
		resetKey();

		// �������� �� ������������ ������
		detectLangShift(_arg.key, false);

		if (isCaptureKey()) mWidgetKeyFocus->_onKeyButtonReleased(_arg.key);

		return isCaptureKey();
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

		if ((keyEvent == OIS::KC_LSHIFT) || (keyEvent == OIS::KC_RSHIFT))
		{
			if (bIsKeyPressed) {
				mIsCharShift = true;
				bIsSecondKeyPressed = true;
				if (bIsFirstKeyPressed) bIsTwoKeyPressed = true;
			} else {
				mIsCharShift = false;
				bIsSecondKeyPressed = false;
				if ((!bIsFirstKeyPressed) && (bIsTwoKeyPressed)) {
					bIsTwoKeyPressed = false;
					// ��������� ����
					changeLanguage();
				}
			}
		}
		else if ((keyEvent == OIS::KC_LMENU) || (keyEvent == OIS::KC_LCONTROL))
		{
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
		else
		{
			bIsFirstKeyPressed = false;
			bIsSecondKeyPressed = false;
			bIsTwoKeyPressed = false;
		}
	}

	wchar_t InputManager::getKeyChar(int keyEvent) // ���������� ������ �� ��� ���� ����
	{
		if (keyEvent < 58) return mCurrentLanguage->second[keyEvent + (mIsCharShift ? 58 : 0)];
		else if (keyEvent < 84) {
			if (keyEvent > 70) return mNums[keyEvent-71];
		} else if (keyEvent == OIS::KC_DIVIDE) return mCurrentLanguage->second[OIS::KC_SLASH + (mIsCharShift ? 58 : 0)];//L'/'; // �� �������� ���������� //????m_langs[m_currentLang][OIS::KC_BACKSLASH + (m_bShiftChars ? 58 : 0)];
		else if (keyEvent == OIS::KC_OEM_102) return mCurrentLanguage->second[OIS::KC_BACKSLASH + (mIsCharShift ? 58 : 0)];
		return 0;
	}

	void InputManager::createDefaultCharSet()
	{
		// ��������� ���������� ����
		mMapLanguages[INPUT_DEFAULT_LANGUAGE] = LangInfo();
		mCurrentLanguage = mMapLanguages.find(INPUT_DEFAULT_LANGUAGE);
		mCurrentLanguage->second.resize(116);

		// ��������� ������
		wchar_t chars[116] = {
			0, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 45, 61, 0, 0, 113, 119, 101, 114, 116, 121, 117, 105, 111, 112, 91, 93, 0, 0, 97, 115, 100, 102, 103, 104, 106, 107, 108, 59, 39, 96, 0, 92, 122, 120, 99, 118, 98, 110, 109, 44, 46, 47, 0, 42, 0, 32, // normal
			0, 0, 33, 64, 35, 36, 37, 94, 38, 42, 40, 41, 95, 43, 0, 0, 81, 87, 69, 82, 84, 89, 85, 73, 79, 80, 123, 125, 0, 0, 65, 83, 68, 70, 71, 72, 74, 75, 76, 58, 34, 126, 0, 124, 90, 88, 67, 86, 66, 78, 77, 60, 62, 63, 0, 42, 0, 32 // shift
		};
		// �������� � ���������� �����
		LangInfo & lang = mCurrentLanguage->second;
		for (size_t i=0; i<116; i++) lang[i] = chars[i];

		// ��������� ������� ��� �������
		wchar_t nums[13] = {55, 56, 57, 45, 52, 53, 54, 43, 49, 50, 51, 48, 46};
		mNums.resize(13);
		// �������� � ���������� �����
		for (size_t i=0; i<13; i++) mNums[i] = nums[i];
	}

	bool InputManager::load(const std::string & _file, bool _resource)
	{
		xml::xmlDocument doc;
		if (false == doc.open((_resource ? helper::getResourcePath(_file) : _file).c_str())) {
			MYGUI_ERROR(doc.getLastError());
			return false;
		}

		xml::xmlNodePtr root = doc.getRoot();
		if ( (root == 0) || (root->getName() != "MyGUI") ) {
			MYGUI_ERROR("not find root tag 'MyGUI'");
			return false;
		}

		std::string type;
		if ( (false == root->findAttribute("type", type)) || (type != "Lang") ) {
			MYGUI_ERROR("not find root type 'Lang'");
			return false;
		}

		xml::xmlNodeIterator lang = root->getNodeIterator();
		while (lang.nextNode("Lang")) {

			std::string name;
			if ( false == lang->findAttribute("name", name)) continue;

			std::vector<std::string> chars = util::split(lang->getBody());
			if (chars.size() == 116) {

				// ������� ��������� ���� �� ����� ���� ���
				MapLang::iterator iter = mMapLanguages.find(name);
				if (iter != mMapLanguages.end()) MYGUI_EXCEPT("name language is exist");

				// ������� ����
				mMapLanguages[name] = LangInfo();
				iter = mMapLanguages.find(name);
				iter->second.resize(116);
				LangInfo & lang = iter->second;

				// � ��������� ���
				for (size_t j=0; j<116; j++) lang[j] = util::parseInt(chars[j]);

			}
			else MYGUI_LOG("count char is not 116");

		};
		// ����������� ��������� ��������, ��� ��� �� �������������� ��� ����������
		mCurrentLanguage = mMapLanguages.find(INPUT_DEFAULT_LANGUAGE);

		return true;
	}

	void InputManager::setKeyFocusWidget(WidgetPtr _widget)
	{

		// ���� ������� �����
		WidgetPtr root = _widget;
		if (root != null) { while (root->getParent() != null) root = root->getParent(); }

		// ���� ������� ����� ���������, �� ���������
		if (mWidgetRootKeyFocus != root) {
			if (mWidgetRootKeyFocus != null) mWidgetRootKeyFocus->_onKeyChangeRootFocus(false);
			if (root != null) root->_onKeyChangeRootFocus(true);
			mWidgetRootKeyFocus = root;
		}

		// � ��� ��� ��� ��������� ������������ �����
		if (_widget == mWidgetKeyFocus) return;

		if (mWidgetKeyFocus != null) mWidgetKeyFocus->_onKeyLostFocus(_widget);
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
		mWidgetMouseFocus = null;
//		mWidgetKeyFocus = null;
		mWidgetRootMouseFocus = null;
//		mWidgetRootKeyFocus = null;
	}

	void InputManager::_frameStarted(float _frame, float _event)
	{
		if ( mHoldKey == OIS::KC_UNASSIGNED) return;
		if ( false == isCaptureKey() ) {
			mHoldKey = OIS::KC_UNASSIGNED;
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
				mWidgetKeyFocus->_onKeyButtonReleased(mHoldKey);
			}
		}

	}

	void InputManager::_frameEnded(float _frame, float _event)
	{
		// ������ �� ������
	}

	void InputManager::storeKey(int _key)
	{
		mHoldKey = OIS::KC_UNASSIGNED;

		if ( false == isCaptureKey() ) return;
		if ( (_key == OIS::KC_LSHIFT)
			|| (_key == OIS::KC_RSHIFT)
			|| (_key == OIS::KC_LCONTROL)
			|| (_key == OIS::KC_RCONTROL)
			|| (_key == OIS::KC_LMENU)
			|| (_key == OIS::KC_RMENU)
			) return;

		mHoldKey = _key;
		mFirstPressKey = true;
		mTimerKey = 0.0f;
	}

	// ������� ������ ������ �� ���� ��������� ����
	void InputManager::unlinkWidget(WidgetPtr _widget)
	{
		if (null == _widget) return;
		if (_widget == mWidgetMouseFocus) {
			mIsWidgetMouseCapture = false;
			mWidgetMouseFocus = null;
		}
		if (_widget == mWidgetKeyFocus) mWidgetKeyFocus = null;
		if (_widget == mWidgetRootMouseFocus) mWidgetRootMouseFocus = null;
		if (_widget == mWidgetRootKeyFocus) mWidgetRootKeyFocus = null;
	}

} // namespace MyGUI
