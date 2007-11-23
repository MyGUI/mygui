
#include "xmlDocument.h"
#include "Common.h"
#include "InputManager.h"
#include "PointerManager.h"
#include "Widget.h"

namespace MyGUI
{

	const std::string INPUT_DEFAULT_LANGUAGE = "English";
	const int INPUT_TIME_DOUBLE_CLICK = 250; //measured in milliseconds
	const float INPUT_DELAY_FIRST_KEY = 0.4f;
	const float INPUT_INTERVAL_KEY = 0.05f;

	INSTANCE_IMPLEMENT(InputManager);

	void InputManager::initialise()
	{
		assert(!mIsInitialise);

		m_widgetMouseFocus = 0;
		m_widgetKeyFocus = 0;
		m_widgetRootMouseFocus = 0;
		m_widgetRootKeyFocus = 0;
		m_isWidgetMouseCapture = false;
		m_isCharShift = false;
		mHoldKey = OIS::KC_UNASSIGNED;
		mIsListener = false;

		createDefaultCharSet();

		mIsInitialise = true;
	}

	void InputManager::shutdown()
	{
		if (!mIsInitialise) return;

		mIsInitialise = false;
	}

	bool InputManager::injectMouseMove( const OIS::MouseEvent & _arg)
	{
		// запоминаем позицию
		mMousePosition.set(_arg.state.X.abs, _arg.state.Y.abs);

		// двигаем курсор
		PointerManager::getInstance().move(_arg.state.X.abs, _arg.state.Y.abs);

		// проверка на скролл
		if (_arg.state.Z.rel != 0) {
			if (m_widgetMouseFocus != null) m_widgetMouseFocus->_onMouseSheel(_arg.state.Z.rel);
			return isCaptureMouse();
		}

		if (m_isWidgetMouseCapture) {
			if (m_widgetMouseFocus != null) m_widgetMouseFocus->_onMouseMove(_arg.state.X.abs, _arg.state.Y.abs);
			else m_isWidgetMouseCapture = false;
			return true;
		}

		// ищем активное окно
		LayerItemInfoPtr rootItem = null;
		WidgetPtr item = static_cast<WidgetPtr>(LayerManager::getInstance().findWidgetItem(_arg.state.X.abs, _arg.state.Y.abs, rootItem));
		if ( (item != null) && (!item->isEnable()) ) item = null; // неактивное окно

		// ничего не изменилось
		if (m_widgetMouseFocus == item) return isCaptureMouse();

		// смена фокуса
		if (m_widgetMouseFocus != null) m_widgetMouseFocus->_onMouseLostFocus(item);
		if (item != null) item->_onMouseSetFocus(m_widgetMouseFocus);

		// изменился рутовый элемент
		if (rootItem != m_widgetRootMouseFocus) {
			if (m_widgetRootMouseFocus != null) m_widgetRootMouseFocus->_onMouseChangeRootFocus(false);
			if (rootItem != null) static_cast<WidgetPtr>(rootItem)->_onMouseChangeRootFocus(true);
			m_widgetRootMouseFocus = static_cast<WidgetPtr>(rootItem);
		}

		// запоминаем текущее окно
		m_widgetMouseFocus = item;

		return isCaptureMouse();
	}

	bool InputManager::injectMousePress( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id )
	{

		if (!isCaptureMouse()) {
			resetKeyFocusWidget();
			return false;
		}

		// захватываем только по левой клавише и только если виджету надо
		if ( _id == OIS::MB_Left ) {
			// захват окна
			m_isWidgetMouseCapture = true;
			// запоминаем место нажатия
			m_lastLeftPressed.left = (int)_arg.state.X.abs;
			m_lastLeftPressed.top = (int)_arg.state.Y.abs;
		}
			
		// устанавливаем перед вызовом т.к. возможно внутри ктонить поменяет фокус под себя
		setKeyFocusWidget(m_widgetMouseFocus);
		m_widgetMouseFocus->_onMouseButtonPressed(_id == OIS::MB_Left);

		// поднимаем виджет, временно
		if (m_widgetMouseFocus != null) {
			WidgetPtr tmp = m_widgetMouseFocus;
			while (tmp->getParent() != null) tmp = tmp->getParent();
			LayerManager::getInstance().upItem(tmp);
		}
		return true;
	}

	bool InputManager::injectMouseRelease( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id )
	{

		if (isCaptureMouse()) {

			// сбрасываем захват
			m_isWidgetMouseCapture = false;

			m_widgetMouseFocus->_onMouseButtonReleased(_id == OIS::MB_Left);

			if ((_id == OIS::MB_Left) && m_time.getMilliseconds() < INPUT_TIME_DOUBLE_CLICK) {
				m_widgetMouseFocus->_onMouseButtonClick(true);
			} else {
			    m_time.reset();
	            m_widgetMouseFocus->_onMouseButtonClick(false);
			}

			// для корректного отображения
			injectMouseMove(_arg);

			return true;
		}

		return false;
	}

	bool InputManager::injectKeyPress(const OIS::KeyEvent & _arg)
	{
		// проверка на переключение языков
		detectLangShift(_arg.key, true);
		
		//Pass keystrokes to the current active text widget
		if (isCaptureKey()) m_widgetKeyFocus->_onKeyButtonPressed(_arg.key, getKeyChar(_arg.key));

		// запоминаем клавишу
		storeKey(_arg.key);

		return isCaptureKey();
	}

	bool InputManager::injectKeyRelease(const OIS::KeyEvent & _arg)
	{
		// сбрасываем клавишу
		resetKey();

		// проверка на переключение языков
		detectLangShift(_arg.key, false);

		if (isCaptureKey()) m_widgetKeyFocus->_onKeyButtonReleased(_arg.key);

		return isCaptureKey();
	}

    //Detects switching from an english to a other mode on a keyboard (?)
	void InputManager::detectLangShift(int keyEvent, bool bIsKeyPressed)
	{
		// если переключать не надо
		if (m_mapLanguages.size() == 1) return;

		// для облегчения распознавания используются LeftAlt+LeftShift или LeftCtrl+LeftShift,LeftShift+LeftAlt или LeftShift+LeftCtrl
		static bool bIsFirstKeyPressed = false; // LeftAlt или LeftCtrl
		static bool bIsSecondKeyPressed = false; // LeftShift
		static bool bIsTwoKeyPressed = false; // обе были зажаты

		if ((keyEvent == OIS::KC_LSHIFT) || (keyEvent == OIS::KC_RSHIFT))
		{
			if (bIsKeyPressed) {
				m_isCharShift = true;
				bIsSecondKeyPressed = true;
				if (bIsFirstKeyPressed) bIsTwoKeyPressed = true;
			} else {
				m_isCharShift = false;
				bIsSecondKeyPressed = false;
				if ((!bIsFirstKeyPressed) && (bIsTwoKeyPressed)) {
					bIsTwoKeyPressed = false;
					// следующий язык
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
					// следующий язык
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

	wchar_t InputManager::getKeyChar(int keyEvent) // возвращает символ по его скан коду
	{
		if (keyEvent < 58) return m_currentLanguage->second[keyEvent + (m_isCharShift ? 58 : 0)];
		else if (keyEvent < 84) {
			if (keyEvent > 70) return m_nums[keyEvent-71];
		} else if (keyEvent == OIS::KC_DIVIDE) return m_currentLanguage->second[OIS::KC_SLASH + (m_isCharShift ? 58 : 0)];//L'/'; // на цифровой клавиатуре //????m_langs[m_currentLang][OIS::KC_BACKSLASH + (m_bShiftChars ? 58 : 0)];
		else if (keyEvent == OIS::KC_OEM_102) return m_currentLanguage->second[OIS::KC_BACKSLASH + (m_isCharShift ? 58 : 0)];
		return 0;
	}

	void InputManager::createDefaultCharSet()
	{
		// вставляем английский язык
		m_mapLanguages[INPUT_DEFAULT_LANGUAGE] = LangInfo();
		m_currentLanguage = m_mapLanguages.find(INPUT_DEFAULT_LANGUAGE);
		m_currentLanguage->second.resize(116);

		// временный массив
		wchar_t chars[116] = {
			0, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 45, 61, 0, 0, 113, 119, 101, 114, 116, 121, 117, 105, 111, 112, 91, 93, 0, 0, 97, 115, 100, 102, 103, 104, 106, 107, 108, 59, 39, 96, 0, 92, 122, 120, 99, 118, 98, 110, 109, 44, 46, 47, 0, 42, 0, 32, // normal
			0, 0, 33, 64, 35, 36, 37, 94, 38, 42, 40, 41, 95, 43, 0, 0, 81, 87, 69, 82, 84, 89, 85, 73, 79, 80, 123, 125, 0, 0, 65, 83, 68, 70, 71, 72, 74, 75, 76, 58, 34, 126, 0, 124, 90, 88, 67, 86, 66, 78, 77, 60, 62, 63, 0, 42, 0, 32 // shift
		};
		// копируем в постоянное место
		LangInfo & lang = m_currentLanguage->second;
		for (size_t i=0; i<116; i++) lang[i] = chars[i];

		// добавляем клавиши для нумлока
		wchar_t nums[13] = {55, 56, 57, 45, 52, 53, 54, 43, 49, 50, 51, 48, 46};
		m_nums.resize(13);
		// копируем в постоянное место
		for (size_t i=0; i<13; i++) m_nums[i] = nums[i];
	}

	void InputManager::load(const std::string & _file)
	{
		xml::xmlDocument doc;
		if (!doc.open(helper::getResourcePath(_file))) OGRE_EXCEPT(0, doc.getLastError(), "");

		xml::xmlNodePtr xml_root = doc.getRoot();
		if ( (xml_root == 0) || (xml_root->getName() != "MyGUI_LangInfo") ) return;

		xml::xmlNodeIterator lang = xml_root->getNodeIterator();
		while (lang.nextNode("Lang")) {

			std::string name;
			if ( false == lang->findAttribute("Name", name)) continue;

			std::vector<std::string> chars = util::split(lang->getBody());
			if (chars.size() == 116) {

				// сначала проверяем есть ли такой язык уже
				MapLang::iterator iter = m_mapLanguages.find(name);
				if (iter != m_mapLanguages.end()) OGRE_EXCEPT(0, "name language is exist", "void InputManager::loadCharSet(const std::string & _file)");

				// создаем язык
				m_mapLanguages[name] = LangInfo();
				iter = m_mapLanguages.find(name);
				iter->second.resize(116);
				LangInfo & lang = iter->second;

				// и заполняем его
				for (size_t j=0; j<116; j++) lang[j] = util::parseInt(chars[j]);

			} else {LOG("count char is not 116");}

		};
		// обязательно обновляем итератор, так как не гарантируеться его сохранение
		m_currentLanguage = m_mapLanguages.find(INPUT_DEFAULT_LANGUAGE);
	}

	void InputManager::setKeyFocusWidget(WidgetPtr _widget)
	{

		// ищем рутовый фокус
		WidgetPtr root = _widget;
		if (root != null) { while (root->getParent() != null) root = root->getParent(); }

		// если рутовый фокус поменялся, то оповещаем
		if (m_widgetRootKeyFocus != root) {
			if (m_widgetRootKeyFocus != null) m_widgetRootKeyFocus->_onKeyChangeRootFocus(false);
			if (root != null) root->_onKeyChangeRootFocus(true);
			m_widgetRootKeyFocus = root;
		}

		// а вот тут уже проверяем обыкновенный фокус
		if (_widget == m_widgetKeyFocus) return;

		if (m_widgetKeyFocus != null) m_widgetKeyFocus->_onKeyLostFocus(_widget);
		if (_widget != null) {
			if (_widget->isNeedKeyFocus()) {
				_widget->_onKeySetFocus(m_widgetKeyFocus);
				m_widgetKeyFocus = _widget;
				return;
			}
		}
		m_widgetKeyFocus = null;

	}

	void InputManager::resetMouseFocusWidget()
	{
		m_widgetMouseFocus = null;
//		m_widgetKeyFocus = null;
		m_widgetRootMouseFocus = null;
//		m_widgetRootKeyFocus = null;
	}

	bool InputManager::frameStarted(const Ogre::FrameEvent& evt)
	{
		if ( mHoldKey == OIS::KC_UNASSIGNED) return true;
		if ( ! isCaptureKey() ) {
			mHoldKey = OIS::KC_UNASSIGNED;
			return true;
		}

		mTimerKey += evt.timeSinceLastFrame;

		if (mFirstPressKey) {
			if (mTimerKey > INPUT_DELAY_FIRST_KEY) {
				mFirstPressKey = false;
				mTimerKey = 0.0f;
			}
		} else {
			if (mTimerKey > INPUT_INTERVAL_KEY) {
				mTimerKey -= INPUT_INTERVAL_KEY;
				m_widgetKeyFocus->_onKeyButtonPressed(mHoldKey, getKeyChar(mHoldKey));
				m_widgetKeyFocus->_onKeyButtonReleased(mHoldKey);
			}
		}

		return true;
	}

	bool InputManager::frameEnded(const Ogre::FrameEvent& evt)
	{
		return true;
	}

} // namespace MyGUI
