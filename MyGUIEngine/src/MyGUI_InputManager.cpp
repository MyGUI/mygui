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
#include "MyGUI_SkinManager.h"
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

		createDefaultCharSet();

		WidgetManager::getInstance().registerUnlinker(this);
		Gui::getInstance().eventFrameStart += newDelegate(this, &InputManager::frameEntered);
		//Gui::getInstance().addFrameListener(newDelegate(this, &InputManager::frameEntered), null);
		ResourceManager::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &InputManager::_load);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;

		mUseOISKeyLayout = false;
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
		// запоминаем позицию
		mMousePosition.set(_absx, _absy);

		// вычисляем прирост по колеса
		int relz = _absz - mOldAbsZ;
		mOldAbsZ = _absz;

		// двигаем курсор
		PointerManager::getInstance().setPosition(mMousePosition);

		// проверка на скролл
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

		// ищем активное окно
		LayerItem *  rootItem = null;
		WidgetPtr item = static_cast<WidgetPtr>(LayerManager::getInstance()._findLayerItem(_absx, _absy, rootItem));

		// спускаемся по владельцу
		if (null != rootItem) {
			while (null != static_cast<WidgetPtr>(rootItem)->_getOwner()) {
				rootItem = static_cast<WidgetPtr>(rootItem)->_getOwner();
			}
		}	

		// ничего не изменилось
		if (mWidgetMouseFocus == item) {
			bool isFocus = isFocusMouse();
			if (mWidgetMouseFocus != null) mWidgetMouseFocus->_onMouseMove(_absx, _absy);
			return isFocus;
		}

		// проверяем на модальность
		if (0 != mVectorModalRootWidget.size()) {
			if (rootItem != mVectorModalRootWidget.back()) {
				rootItem = null;
				item = null;
			}
		}

		// смена фокуса, проверяем на доступность виджета
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
		// сбрасываем курсор
		else if (false == mPointer.empty()) {
			PointerManager::getInstance().setDefaultPointer();
			mPointer.clear();

			eventChangeMousePointer(PointerManager::getInstance().getDefaultPointer());
		}

		// изменился рутовый элемент
		if (rootItem != mWidgetRootMouseFocus) {
			if (mWidgetRootMouseFocus != null) mWidgetRootMouseFocus->_onMouseChangeRootFocus(false);
			if (rootItem != null) static_cast<WidgetPtr>(rootItem)->_onMouseChangeRootFocus(true);
			mWidgetRootMouseFocus = static_cast<WidgetPtr>(rootItem);
		}

		// запоминаем текущее окно
		mWidgetMouseFocus = item;

		// обновляем данные
		updateFocusWidgetHelpers();

		return isFocusMouse();
	}

	bool InputManager::injectMousePress(int _absx, int _absy, MouseButton _id)
	{

		// если мы щелкнули не на гуй
		if (false == isFocusMouse()) {
			resetKeyFocusWidget();
			return false;
		}

		// если активный элемент заблокирован
		if (false == mWidgetMouseFocus->isEnabled()) return true;

		// захватываем только по левой клавише и только если виджету надо
		if (MB_Left == _id) {
			// захват окна
			mIsWidgetMouseCapture = true;
			// запоминаем место нажатия
			mLastLeftPressed.set(_absx, _absy);
		}

		// ищем вверх тот виджет который может принимать фокус
		WidgetPtr focus = mWidgetMouseFocus;
		while ((focus != null) && (false == focus->isNeedKeyFocus())) focus = focus->getParent();

		// устанавливаем перед вызовом т.к. возможно внутри ктонить поменяет фокус под себя
		setKeyFocusWidget(focus);

		if (mWidgetMouseFocus != null) {

			mWidgetMouseFocus->_onMouseButtonPressed(_absx, _absy, _id);
			//mWidgetMouseFocus->_onMouseButtonPressed(_id);

			// поднимаем виджет
			LayerManager::getInstance().upLayerItem(mWidgetMouseFocus);
		}
		return true;
	}

	bool InputManager::injectMouseRelease(int _absx, int _absy, MouseButton _id)
	{

		if (isFocusMouse()) {

			// если активный элемент заблокирован
			if (false == mWidgetMouseFocus->isEnabled()) return true;

			mWidgetMouseFocus->_onMouseButtonReleased(_absx, _absy, _id);

			if (mIsWidgetMouseCapture) {

				// сбрасываем захват
				mIsWidgetMouseCapture = false;

				// после вызова, виджет может быть удален
				if (null != mWidgetMouseFocus) {

					if ((MB_Left == _id) && mTime.getMilliseconds() < INPUT_TIME_DOUBLE_CLICK) {
						mWidgetMouseFocus->_onMouseButtonDoubleClick();
					}
					else {
						// проверяем над тем ли мы окном сейчас что и были при нажатии
						LayerItem * rootItem = null;
						WidgetPtr item = static_cast<WidgetPtr>(LayerManager::getInstance()._findLayerItem(_absx, _absy, rootItem));
						if ( item == mWidgetMouseFocus) {
							mWidgetMouseFocus->_onMouseButtonClick();
						}
						mTime.reset();
					}
				}
			}

			// для корректного отображения
			injectMouseMove(_absx, _absy, mOldAbsZ);

			return true;
		}

		return false;
	}

	bool InputManager::injectKeyPress(KeyCode _key)
	{
		// проверка на переключение языков
		detectLangShift(_key, true);
		// запоминаем клавишу
		storeKey(_key);

		bool wasFocusKey = isFocusKey();

		//Pass keystrokes to the current active text widget
		if (isFocusKey()) {
			mWidgetKeyFocus->_onKeyButtonPressed(_key, getKeyChar(_key));
		}

		return wasFocusKey;
	}

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
	static WCHAR deadKey = 0;
	int _translateText( KeyCode kc )
	{
		BYTE keyState[256];
		HKL  layout = GetKeyboardLayout(0);
		if( GetKeyboardState(keyState) == 0 )
			return 0;

		unsigned int vk = MapVirtualKeyEx(kc, 3, layout);
		if( vk == 0 )
			return 0;

		//unsigned char buff[3] = {0,0,0};
		//int ascii = ToAsciiEx(vk, kc, keyState, (LPWORD) buff, 0, layout);
		WCHAR buff[3]={0,0,0};
		int ascii = ToUnicodeEx(vk, kc, keyState, buff, 3, 0, layout);
		if(ascii == 1 && deadKey != '\0' )
		{
			// A dead key is stored and we have just converted a character key
			// Combine the two into a single character
			WCHAR wcBuff[3] = {buff[0], deadKey, '\0'};
			WCHAR out[3];
			
			deadKey = '\0';
			if(FoldStringW(MAP_PRECOMPOSED, (LPWSTR)wcBuff, 3, (LPWSTR)out, 3))
				return out[0];
		}
		else if (ascii == 1)
		{	// We have a single character
			deadKey = '\0';
			return buff[0];
		}
		else if(ascii == 2)
		{	// Convert a non-combining diacritical mark into a combining diacritical mark
			// Combining versions range from 0x300 to 0x36F; only 5 (for French) have been mapped below
			// http://www.fileformat.info/info/unicode/block/combining_diacritical_marks/images.htm
			switch(buff[0])	{
			case 0x5E: // Circumflex accent: в
				deadKey = 0x302; break;
			case 0x60: // Grave accent: а
				deadKey = 0x300; break;
			case 0xA8: // Diaeresis: ь
				deadKey = 0x308; break;
			case 0xB4: // Acute accent: й
				deadKey = 0x301; break;
			case 0xB8: // Cedilla: з
				deadKey = 0x327; break;
			default:
				deadKey = buff[0]; break;
			}
		}

		return 0;
	}
#endif

	bool InputManager::injectKeyPress(KeyCode _key, unsigned int _text)
	{
		// проверка на переключение языков
		detectLangShift(_key, true);
		// запоминаем клавишу
		storeKey(_key);

		bool wasFocusKey = isFocusKey();

		//Pass keystrokes to the current active text widget
		if (isFocusKey()) {
			Char ch;
			if (mUseOISKeyLayout) {
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
				ch = _translateText(_key);
#else
				ch = _text;
#endif
			}
			else {
				ch = getKeyChar(_key);
			}
			mWidgetKeyFocus->_onKeyButtonPressed(_key, ch);
		}

		return wasFocusKey;
	}

	bool InputManager::injectKeyRelease(KeyCode _key)
	{
		// проверка на переключение языков
		detectLangShift(_key, false);
		// сбрасываем клавишу
		resetKey(_key);

		bool wasFocusKey = isFocusKey();

		if (isFocusKey()) mWidgetKeyFocus->_onKeyButtonReleased(_key);

		return wasFocusKey;
	}

    //Detects switching from an english to a other mode on a keyboard (?)
	void InputManager::detectLangShift(KeyCode keyEvent, bool bIsKeyPressed)
	{
		return;
		// если переключать не надо
		if (mMapLanguages.size() == 1) return;

		// для облегчения распознавания используются LeftAlt+LeftShift или LeftCtrl+LeftShift,LeftShift+LeftAlt или LeftShift+LeftCtrl
		static bool bIsFirstKeyPressed = false; // LeftAlt или LeftCtrl
		static bool bIsSecondKeyPressed = false; // LeftShift
		static bool bIsTwoKeyPressed = false; // обе были зажаты

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
					// следующий язык
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
					// следующий язык
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

	Char InputManager::getKeyChar(KeyCode keyEvent) // возвращает символ по его скан коду
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
		// вставляем английский язык
		mMapLanguages[INPUT_DEFAULT_LANGUAGE] = LangInfo();
		mCurrentLanguage = mMapLanguages.find(INPUT_DEFAULT_LANGUAGE);
		mCurrentLanguage->second.resize(INPUT_COUNT_LOAD_CHAR);

		// временный массив
		Char chars[INPUT_COUNT_LOAD_CHAR] = {
			0, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 45, 61, 0, 0, 113, 119, 101, 114, 116, 121, 117, 105, 111, 112, 91, 93, 0, 0, 97, 115, 100, 102, 103, 104, 106, 107, 108, 59, 39, 96, 0, 92, 122, 120, 99, 118, 98, 110, 109, 44, 46, 47, 0, 42, 0, 32, // normal
			0, 0, 33, 64, 35, 36, 37, 94, 38, 42, 40, 41, 95, 43, 0, 0, 81, 87, 69, 82, 84, 89, 85, 73, 79, 80, 123, 125, 0, 0, 65, 83, 68, 70, 71, 72, 74, 75, 76, 58, 34, 126, 0, 124, 90, 88, 67, 86, 66, 78, 77, 60, 62, 63, 0, 42, 0, 32 // shift
		};
		// копируем в постоянное место
		LangInfo & lang = mCurrentLanguage->second;
		for (size_t i=0; i<INPUT_COUNT_LOAD_CHAR; i++) lang[i] = chars[i];

		// добавляем клавиши для нумлока
		Char nums[13] = {55, 56, 57, 45, 52, 53, 54, 43, 49, 50, 51, 48, 46};
		mNums.resize(13);
		// копируем в постоянное место
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

				// сначала проверяем есть ли такой язык уже
				MapLang::iterator iter = mMapLanguages.find(name);
				MYGUI_ASSERT(iter == mMapLanguages.end(), "language '" << name << "' already exist");

				// создаем язык
				mMapLanguages[name] = LangInfo();
				iter = mMapLanguages.find(name);
				iter->second.resize(INPUT_COUNT_LOAD_CHAR);
				LangInfo & lang = iter->second;

				// и заполняем его
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

		// обязательно обновляем итератор, так как не гарантируеться его сохранение
		mCurrentLanguage = mMapLanguages.find(INPUT_DEFAULT_LANGUAGE);
	}

	void InputManager::setKeyFocusWidget(WidgetPtr _widget)
	{
		// ищем рутовый фокус
		WidgetPtr root = _widget;
		if (root != null) { while (root->_getOwner() != null) root = root->_getOwner(); }

		// если рутовый фокус поменялся, то оповещаем
		if (mWidgetRootKeyFocus != root) {
			if (mWidgetRootKeyFocus != null) mWidgetRootKeyFocus->_onKeyChangeRootFocus(false);
			if (root != null) root->_onKeyChangeRootFocus(true);
			mWidgetRootKeyFocus = root;
		}

		// а вот тут уже проверяем обыкновенный фокус
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

		// обновляем данные
		updateFocusWidgetHelpers();

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

		// обновляем данные
		updateFocusWidgetHelpers();

	}

	// удаляем данный виджет из всех возможных мест
	void InputManager::_unlinkWidget(WidgetPtr _widget)
	{
		if (null == _widget) return;
		if (_widget == mWidgetMouseFocus) {
			mIsWidgetMouseCapture = false;
			mWidgetMouseFocus = null;

			// обновляем данные
			updateFocusWidgetHelpers();
		}
		if (_widget == mWidgetKeyFocus) {
			mWidgetKeyFocus = null;

			// обновляем данные
			updateFocusWidgetHelpers();
		}
		if (_widget == mWidgetRootMouseFocus) mWidgetRootMouseFocus = null;
		if (_widget == mWidgetRootKeyFocus) mWidgetRootKeyFocus = null;

		// ручками сбрасываем, чтобы не менять фокусы
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
		// если еще есть модальные то их фокусируем и поднимаем
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

		// обновляем данные
		updateFocusWidgetHelpers();

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
	void InputManager::updateFocusWidgetHelpers(bool * _show, bool _set)
	{

		static bool show = false;

		// запрос на видимость
		if (_show) {
			if (_set) show = *_show;
			else *_show = show;
			return;
		}

		if (!show) return;

		static const std::string layer = "Statistic";
		static const std::string skin_mouse = "RectGreen";
		static const std::string skin_key = "RectBlue";

		static WidgetPtr mouse_focus = null;
		static WidgetPtr mouse_helper = null;
		if ((mWidgetMouseFocus != mouse_focus) || ((mWidgetMouseFocus != null) && (mouse_helper != null) && mWidgetMouseFocus->getAbsoluteCoord() != mouse_helper->getAbsoluteCoord())) {
			mouse_focus = mWidgetMouseFocus;

			if (mouse_helper == null) {
				if (!LayerManager::getInstance().isExist(layer)) return;
				if (!SkinManager::getInstance().isExist(skin_mouse)) return;
				mouse_helper = Gui::getInstance().createWidget<Widget>(skin_mouse, IntCoord(), Align::Default, layer);
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
				if (!SkinManager::getInstance().isExist(skin_key)) return;
				key_helper = Gui::getInstance().createWidget<Widget>(skin_key, IntCoord(), Align::Default, layer);
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
