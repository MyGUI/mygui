#include "MyGUI.h"
#include "MyGUI_Logging.h"
#include <OgreStringConverter.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreTechnique.h>

using namespace Ogre;
using namespace std;
using namespace OIS;

namespace MyGUI {

    GUI::GUI() { }
	GUI *GUI::Initialize(uint16 uWidth, uint16 uHeight, EventCallback *pEventCallback)
	{
		m_uWidth = (uWidth);
		m_uHeight = (uHeight);
		m_uMaxZOrder = (__GUI_ZORDER_OVERLAPPED); // начало для перекрывающихся окон
		m_bShiftChars = (0);
		m_bIsActiveGUI = (false);
		m_bIsFocusWindowCapture = (0);
		m_currentWindow = (0);
		m_currentFocusWindow = (0);
		m_currentEditWindow = (0);
		m_uOverlappedStart = (0);
		m_uOverlappedEnd = (0);
		m_pEventCallback = (pEventCallback);
		m_bFadeState = (FADE_END);
		m_currentFadeWindow = (0);
		m_pEventCallbackFade = (0);
		
		_LOG("create MyGui object (%p) ,  %d x %d\r\n", this, uWidth, uHeight);

		m_overlayGUI[OVERLAY_FADE] = createOverlay("MyGUI_"+StringConverter::toString(OVERLAY_FADE), __GUI_ZORDER_FADE, false);
		m_overlayGUI[OVERLAY_MOUSE] = createOverlay("MyGUI_"+StringConverter::toString(OVERLAY_MOUSE), __GUI_ZORDER_MOUSE);
		m_overlayGUI[OVERLAY_POPUP] = createOverlay("MyGUI_"+StringConverter::toString(OVERLAY_POPUP), __GUI_ZORDER_POPUP);
		m_overlayGUI[OVERLAY_MAIN] = createOverlay("MyGUI_"+StringConverter::toString(OVERLAY_MAIN), __GUI_ZORDER_MAIN);
		m_overlayGUI[OVERLAY_BACK] = createOverlay("MyGUI_"+StringConverter::toString(OVERLAY_BACK), __GUI_ZORDER_BACK);

		SkinManager::getSingleton()->createSkin();

		createMousePointer();
		setMousePointer(POINTER_DEFAULT);
		
		return this;
	}

	GUI::~GUI()
	{
		while (m_aWindowChild.size() > 0) destroyWindow(m_aWindowChild[0]);
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		m_overlayGUI[OVERLAY_MOUSE]->remove2D(m_overlayContainerMouse);
		overlayManager.destroyOverlayElement(m_overlayContainerMouse);
		for (uint16 pos=0; pos<__OVERLAY_COUNT; pos++) overlayManager.destroy(m_overlayGUI[pos]);

		_LOG("\r\ndestroy MyGui object (%p)", this);
	}

	void GUI::destroyWindow(__MYGUI_OVERLAYS overlay) // уничтожает окно и удаляет из списка
	{
		if (overlay == OVERLAY_DESTROY_ALL) {
			// все окна
			while (m_aWindowChild.size() > 0) destroyWindow(m_aWindowChild[0]);
		} else if (overlay == OVERLAY_OVERLAPPED) {
			// все перекрывающиеся
			while (m_uOverlappedStart < m_uOverlappedEnd) destroyWindow(m_aWindowChild[m_uOverlappedEnd-1]);
		} else if ((overlay < __OVERLAY_COUNT) && (overlay != OVERLAY_MOUSE)) {
			// поиск всех окон этого оверлея
			int16 pos = 0;
			int16 size = (int16)m_aWindowChild.size();
			while (pos < size) {
				if (m_aWindowChild[pos]->m_overlay == m_overlayGUI[overlay]) {
					destroyWindow(m_aWindowChild[pos]);
					pos--;
					size --;
				}
				pos ++;
			}
		}
	}

	void GUI::destroyWindow(Window * pWindow) // уничтожает окно и удаляет из списка
	{
		if (!pWindow) return;

		// сброс всех активных окон, в конце функции все восстановится кроме захвата мыши
		m_currentWindow = 0; // сброс активного фрейма
		m_currentEditWindow = 0;
		m_bIsFocusWindowCapture = false; // сброс захвата
		m_currentFocusWindow = 0; // сброс активного окна

		uint16 size;
		if (pWindow->m_pWindowParent) { // есть отец
			Window * pParentWindow = pWindow->m_pWindowParent;
			size = (uint16)pParentWindow->m_aWindowChild.size();
			for (uint16 i=0; i<size; i++) { // ищем в детском саду
				if (pWindow == pParentWindow->m_aWindowChild[i]) { // нашелся
					pParentWindow->m_aWindowChild[i] = pParentWindow->m_aWindowChild[size-1];
					pParentWindow->m_aWindowChild.pop_back();
					delete pWindow; // удаляем окно
					i = size; // выходим
				}
			}
		} else  { // ребенок гуи
			upZOrder(pWindow); // поднимаем его
			size = (uint16)m_aWindowChild.size();
			for (uint16 i=0; i<size; i++) { // ищем в детском саду
				if (pWindow != m_aWindowChild[i]) continue; // не тот

				if (i != 0) { // попробуем известить предыдущее окно о том что оно теперь выше всех перекрывающихся
					Window * pWindowBack = m_aWindowChild[i-1];
					if (pWindowBack->m_bIsOverlapped) pWindowBack->_OnUpZOrder();
				}

				if (pWindow->m_bIsOverlapped) { // есть свой оверлей
					m_uMaxZOrder -= __GUI_ZORDER_OVERLAPPED_STEP; // уменьшаем оверлеи
					m_uOverlappedEnd --;
				} else { // принадлежит оверлеям гуи
					__ASSERT(pWindow->m_overlay != 0); // как так
					if (pWindow->m_overlay->getZOrder() < __GUI_ZORDER_OVERLAPPED) { // слои ниже перекрывающихся
						m_uOverlappedStart --;
						m_uOverlappedEnd --;
					}
				}

				for (uint16 pos=i; pos<(size-1); pos++) m_aWindowChild[pos] = m_aWindowChild[pos+1];
				m_aWindowChild.pop_back();

				delete pWindow; // удаляем окно
				i = size; // выходим
			}
		}

		// просчет активности окон, для правильности отображения
//		eventMouseMove(m_overlayContainerMouse->getLeft()/m_uWidth, 0.0, m_overlayContainerMouse->getTop()/m_uHeight, false);
		// посылается уведомление, !!! указатель окна мертвый
		if (m_pEventCallback) m_pEventCallback->onWarningEvent(pWindow, WE_WARNING_CHILD_DELETE);
	}

	void GUI::createMousePointer()
	{
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		m_overlayContainerMouse = static_cast<OverlayContainer*>(overlayManager.createOverlayElement("Panel", "MyGUI_overlayContainerMouse"));
		m_overlayContainerMouse->setMetricsMode(GMM_PIXELS);
		m_overlayContainerMouse->setPosition(0.0, 0.0);
		m_overlayContainerMouse->setDimensions(
		    SkinManager::getSingleton()->getPointer(POINTER_DEFAULT)->uSizeX,
		    SkinManager::getSingleton()->getPointer(POINTER_DEFAULT)->uSizeY);
		m_overlayContainerMouse->show();
		m_overlayGUI[OVERLAY_MOUSE]->add2D(m_overlayContainerMouse);
		m_overlayGUI[OVERLAY_MOUSE]->show();

	}

	void GUI::setMousePointer(const Ogre::String uTypePointer) // изменить указатель
	{
		uPointerType = uTypePointer;
		if (SkinManager::getSingleton()->getPointer(uTypePointer)->strMaterialName == "")
		    return;
		m_overlayContainerMouse->setMaterialName(
		    SkinManager::getSingleton()->getPointer(uTypePointer)->strMaterialName);
	}

	void GUI::upZOrder(Window *pWindow) // поднять окно по слоям вверх
	{ // поднятие и упорядочивание окон в массиве , для увеличения скорости просчетов
		if (!pWindow->m_bIsOverlapped) return; // ходют тут всякие
		 // перебираем все перекрывающиеся окна
		for (uint16 pos = m_uOverlappedStart; pos<(m_uOverlappedEnd-1); pos++) {
			if (pWindow == m_aWindowChild[pos]) { // если это наше окно, то меняем его с верхним
				m_aWindowChild[pos] = m_aWindowChild[pos+1];
				m_aWindowChild[pos+1] = pWindow;
				uint16 uZOrder = pWindow->m_overlay->getZOrder();
				pWindow->m_overlay->setZOrder(m_aWindowChild[pos]->m_overlay->getZOrder());
				m_aWindowChild[pos]->m_overlay->setZOrder(uZOrder);
			}
		}
	}

	Window * GUI::getTopWindow() // возвращает самое верхнее окно из перекрывающихся
	{
		if (m_uOverlappedStart == m_uOverlappedEnd) return 0;
		return m_aWindowChild[m_uOverlappedEnd-1];
	}

	void GUI::setKeyFocus(Window * pWindow) // ставим фокус ввода
	{
		if (m_currentEditWindow) m_currentEditWindow->_OnKeyChangeFocus(false);
		m_currentEditWindow = pWindow;
		if (m_currentEditWindow) m_currentEditWindow->_OnKeyChangeFocus(true);
	}


	bool GUI::mouseReleased( const OIS::MouseEvent &arg , OIS::MouseButtonID id )
	{
	    static Ogre::Timer m_time;
		if (m_bIsFocusWindowCapture) { // окно было захваченно
			m_bIsFocusWindowCapture = false;
			m_currentFocusWindow->_OnMouseButtonPressed(false);
			Window * pOldWindow = m_currentFocusWindow;
//				eventMouseMove(iPosX, iPosY, 0.0, leftMouseButtonDown); // для коректности отображения
			if (pOldWindow == m_currentFocusWindow) {
				m_currentFocusWindow->_OnMouseButtonClick(false); // посылаем всегда
				if (m_time.getMilliseconds() < __GUI_TIME_DOUBLE_CLICK) {
					// окно может быть удаленно после клика
					if (m_currentFocusWindow) m_currentFocusWindow->_OnMouseButtonClick(true);
				}
				else m_time.reset(); // защита от тройных щелчков
			}
		}
		return m_bIsActiveGUI;
	}

	bool GUI::mousePressed( const OIS::MouseEvent &arg , OIS::MouseButtonID id )
	{
		if (id != OIS::MB_Left) return m_bIsActiveGUI;

		if (!m_bIsActiveGUI) {
			if (m_currentEditWindow) { // сброс окна ввода
				m_currentEditWindow->_OnKeyChangeFocus(false);
				m_currentEditWindow = 0;
			}
			return false;
		}

		if (!m_currentWindow) return false;
		upZOrder(m_currentWindow); // поднятие окна над другими
		
		if (m_currentFocusWindow) {
			m_bIsFocusWindowCapture = true; // захват окна

			// смещение клика внутри окна
			m_iOffsetPressedX = arg.state.X.abs - ((int16)(m_currentFocusWindow->m_overlayContainer->_getDerivedLeft()*m_uWidth)) - 1;
			m_iOffsetPressedY = arg.state.Y.abs - ((int16)(m_currentFocusWindow->m_overlayContainer->_getDerivedTop()*m_uHeight)) - 1;

			m_currentFocusWindow->_OnMouseButtonPressed(true);
		}

		if (m_currentEditWindow != m_currentFocusWindow) { // смена окна ввода
			if (m_currentEditWindow) m_currentEditWindow->_OnKeyChangeFocus(false);
			m_currentEditWindow = m_currentFocusWindow;
			if (m_currentEditWindow) m_currentEditWindow->_OnKeyChangeFocus(true);
		}

		m_currentWindow->_OnUpZOrder(); // уведомление об поднятии

		return true;
	}

	bool GUI::mouseMoved( const OIS::MouseEvent &arg )
	{
		if (arg.state.Z.rel != 0) { // скролл
			if (m_currentFocusWindow) {
				if (arg.state.Z.rel < 0.0) m_currentFocusWindow->_OnKeyButtonPressed(KC_WEBFORWARD, 0);
				else m_currentFocusWindow->_OnKeyButtonPressed(KC_WEBBACK, 0);
			}
			return m_bIsActiveGUI;
		}

		m_bIsActiveGUI = false;

		if (m_bIsFocusWindowCapture) {
			m_currentFocusWindow->_OnMouseMove(arg.state.X.abs, arg.state.Y.abs);
			m_bIsActiveGUI =  true;

		} else {
			static Window * pOldFocusWindow = 0;
			pOldFocusWindow = m_currentFocusWindow;
			m_currentFocusWindow = 0; // есть кнопка, сбросим
			m_currentWindow = 0; // текущее окно

			// ищем окно над которым курсор, из дочек гуи
			for (int16 i=(((int16)m_aWindowChild.size())-1); i>=0; i--) { // окна в массиве упорядоченны по zOrder
				if(m_aWindowChild[i]->check(arg.state.X.abs, arg.state.Y.abs, true)) {
					m_currentWindow = m_aWindowChild[i];
					m_bIsActiveGUI = true;
					i = -1; // выход из цикла
				}
			}

			if (m_currentFocusWindow != pOldFocusWindow) { // изменилась активное окно
				if (pOldFocusWindow) pOldFocusWindow->_OnMouseChangeFocus(false);
				if (m_currentFocusWindow) {
					m_currentFocusWindow->_OnMouseChangeFocus(true);
				}
			}

		}

		m_overlayContainerMouse->setPosition(
		    arg.state.X.abs + SkinManager::getSingleton()->getPointer(GUI::getSingleton()->uPointerType)->iOffsetX,
            arg.state.Y.abs + SkinManager::getSingleton()->getPointer(GUI::getSingleton()->uPointerType)->iOffsetY);

		return m_bIsActiveGUI;
	}

	void GUI::detectLangShift(int keyEvent, bool bIsKeyPressed)
	{
		#define __INPUT_SHIFT_MASK 0x01
		#define __INPUT_LANG_MASK 0x02
		// для облегчения распознавания используются LeftAlt+LeftShift или LeftCtrl+LeftShift,LeftShift+LeftAlt или LeftShift+LeftCtrl
		static bool bIsFirstKeyPressed = false; // LeftAlt или LeftCtrl
		static bool bIsSecondKeyPressed = false; // LeftShift
		static bool bIsTwoKeyPressed = false; // обе были зажаты

		if ((keyEvent == KC_LSHIFT) || (keyEvent == KC_RSHIFT)) {
			if (bIsKeyPressed) {
				m_bShiftChars |= __INPUT_SHIFT_MASK;
				bIsSecondKeyPressed = true;
				if (bIsFirstKeyPressed) bIsTwoKeyPressed = true;
			} else {
				m_bShiftChars &= ~__INPUT_SHIFT_MASK;
				bIsSecondKeyPressed = false;
				if ((!bIsFirstKeyPressed) && (bIsTwoKeyPressed)) {
					bIsTwoKeyPressed = false;
					if (m_bShiftChars & __INPUT_LANG_MASK) m_bShiftChars &= ~__INPUT_LANG_MASK;
					else m_bShiftChars |= __INPUT_LANG_MASK;
				}
			}
		} else if ((keyEvent == KC_LMENU) || (keyEvent == KC_LCONTROL)) {
			if (bIsKeyPressed) {
				bIsFirstKeyPressed = true;
				if (bIsSecondKeyPressed) bIsTwoKeyPressed = true;
			} else {
				bIsFirstKeyPressed = false;
				if ((!bIsSecondKeyPressed) && (bIsTwoKeyPressed)) {
					bIsTwoKeyPressed = false;
					if (m_bShiftChars & __INPUT_LANG_MASK) m_bShiftChars &= ~__INPUT_LANG_MASK;
					else m_bShiftChars |= __INPUT_LANG_MASK;
				}
			}
		} else {
			bIsFirstKeyPressed = false;
			bIsSecondKeyPressed = false;
			bIsTwoKeyPressed = false;
		}

	}
	
	bool GUI::keyPressed( const OIS::KeyEvent &arg ) // вызывать при нажатии кнопок клавы
	{
		detectLangShift(arg.key, true); // проверка на переключение языков
		if (!m_currentEditWindow)
		    return false;
		m_currentEditWindow->_OnKeyButtonPressed(arg.key, getKeyChar(arg.key));
		return true;
	}

	bool GUI::keyReleased( const OIS::KeyEvent &arg ) // вызывать при отпускании кнопок клавы
	{
		detectLangShift(arg.key, false); // проверка на переключение языков
		if (!m_currentEditWindow) return false;
		return true;
	}

	void GUI::eventWindowResize(uint16 uWidth, uint16 uHeight) // изменился размер главного окна
	{
		m_uWidth = uWidth;
		m_uHeight = uHeight;
		 // затемнение
		if (m_currentFadeWindow) m_currentFadeWindow->size(uWidth, uHeight);
		// окно сообщений
		Overlay::Overlay2DElementsIterator iter = m_overlayGUI[OVERLAY_FADE]->get2DElementsIterator();
		if (iter.hasMoreElements()) {
			iter.getNext()->setDimensions(uWidth, uHeight); // может еще и данные размеров класса Window поменять
		}
	}

	void GUI::eventUpdateAnimation(Real fTimeLastFrame) // обновляет анимацию гуи
	{
		if (m_bFadeState != FADE_END) {
			if (m_bFadeState == FADE_UP) { // рассвет
				m_fCurrentFadeAlpha -= fTimeLastFrame;
				if (m_fCurrentFadeAlpha <= 0.0) {
					m_bFadeState = FADE_END;
					m_overlayGUI[OVERLAY_FADE]->hide(); // скрываем оверлей файдинга
					destroyWindow(m_currentFadeWindow); // уничтожаем окно затемнения
					m_currentFadeWindow = 0;
					if (m_pEventCallbackFade) m_pEventCallbackFade->onFadeEnd(false, m_uFadeID);
					else if (m_pEventCallback) m_pEventCallback->onFadeEnd(false, m_uFadeID);
					// просчет активности окон, для правильности отображения
//					eventMouseMove(m_overlayContainerMouse->getLeft()/m_uWidth, 0.0, m_overlayContainerMouse->getTop()/m_uHeight, false);
				} else { // покажем изменения
					MaterialPtr Mat = m_currentFadeWindow->m_overlayContainer->getMaterial();
					TextureUnitState* texunit = Mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
					texunit->setAlphaOperation(LBX_MODULATE, LBS_TEXTURE, LBS_MANUAL, 1.0, m_fCurrentFadeAlpha);
				}
			} else { // закат
				m_fCurrentFadeAlpha += fTimeLastFrame;
				if (m_fCurrentFadeAlpha >= 1.0) {
					m_bFadeState = FADE_END;
					m_overlayGUI[OVERLAY_FADE]->hide(); // скрываем оверлей файдинга
					destroyWindow(m_currentFadeWindow); // уничтожаем окно затемнения
					m_currentFadeWindow = 0;
					if (m_pEventCallbackFade) m_pEventCallbackFade->onFadeEnd(true, m_uFadeID);
					else if (m_pEventCallback) m_pEventCallback->onFadeEnd(true, m_uFadeID);
					// просчет активности окон, для правильности отображения
//					eventMouseMove(m_overlayContainerMouse->getLeft()/m_uWidth, 0.0, m_overlayContainerMouse->getTop()/m_uHeight, false);
				} else {  // покажем изменения
					MaterialPtr Mat = m_currentFadeWindow->m_overlayContainer->getMaterial();
					TextureUnitState* texunit = Mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
					texunit->setAlphaOperation(LBX_MODULATE, LBS_TEXTURE, LBS_MANUAL, 1.0, m_fCurrentFadeAlpha);
				}
			}
		}

	}

	void GUI::fadeScreen(bool bIsFade, uint8 uFadeID, EventCallback *pEventCallback) // затеняем экран
	{
		m_pEventCallbackFade = pEventCallback; // для вызова функции
		m_uFadeID = uFadeID;
		if (bIsFade) { // закат
			m_bFadeState = FADE_DOWN;
			m_fCurrentFadeAlpha = 0.0;
		} else { //APPEAR - появиться
			m_bFadeState = FADE_UP;
			m_fCurrentFadeAlpha = 1.0;
		}

		m_overlayGUI[OVERLAY_FADE]->show(); // показываем оверлей файдинга
		m_overlayGUI[OVERLAY_FADE]->setZOrder(__GUI_ZORDER_FADE); // поднимает вверх

		__ASSERT(m_currentFadeWindow == 0);
		m_currentFadeWindow = spawn<Window>(Coord(0, 0), Coord(m_uWidth, m_uHeight), OVERLAY_FADE, SKIN_FADE);

		MaterialPtr Mat = m_currentFadeWindow->m_overlayContainer->getMaterial();
		TextureUnitState* texunit = Mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
		texunit->setAlphaOperation(LBX_MODULATE, LBS_TEXTURE, LBS_MANUAL, 1.0, m_fCurrentFadeAlpha);

	}

	Overlay * GUI::createOverlay(String strName, uint16 zOrder, bool bIsShow) // создает оверлей
	{
		_LOG("create Overlay '%s' \t zOrder = %d", strName.c_str(), zOrder);

		static String old_name;
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		Overlay * pOverlay = overlayManager.create(strName);
		pOverlay->setZOrder(zOrder);
		if (bIsShow) pOverlay->show();

		// напоминаем старому окну его место
		Overlay * pOldOverlay = 0;
		try {pOldOverlay = overlayManager.getByName(old_name);} catch (...) {}
		if (pOldOverlay) {
			pOldOverlay->setZOrder(pOldOverlay->getZOrder());
		}
		old_name = strName;
		return pOverlay;
	}

	wchar_t GUI::getKeyChar(int keyEvent) // возвращает символ по его скан коду
	{
		static wchar_t s_chars[4][58] = { // четыре набора по 58 символов
			// латинские буквы
			0,0,L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',L'0',L'-',L'=',0,0,L'q',L'w',L'e',L'r',L't',L'y',L'u',L'i',L'o',L'p',L'[',L']',
			0,0,L'a',L's',L'd',L'f',L'g',L'h',L'j',L'k',L'l',L';',L'\'',L'`',0,L'\\',L'z',L'x',L'c',L'v',L'b',L'n',L'm',L',',L'.',L'/',0,L'*',0,L' ',
			// латинские буквы в верхнем регистре
			0,0,L'!',L'@',L'#',L'$',L'%',L'^',L'&',L'*',L'(',L')',L'_',L'+',0,0,L'Q',L'W',L'E',L'R',L'T',L'Y',L'U',L'I',L'O',L'P',L'{',L'}',
			0,0,L'A',L'S',L'D',L'F',L'G',L'H',L'J',L'K',L'L',L':',L'"',L'~',0,L'|',L'Z',L'X',L'C',L'V',L'B',L'N',L'M',L'<',L'>',L'?',0,L'*',0,L' ',
			// русские буквы
			0,0,L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',L'0',L'-',L'=',0,0,L'й',L'ц',L'у',L'к',L'е',L'н',L'г',L'ш',L'щ',L'з',L'х',L'ъ',
			0,0,L'ф',L'ы',L'в',L'а',L'п',L'р',L'о',L'л',L'д',L'ж',L'э',L'ё',0,L'\\',L'я',L'ч',L'с',L'м',L'и',L'т',L'ь',L'б',L'ю',L'.',0,L'*',0,L' ',
			// русские буквы в верхнем регистре
			0,0,L'!',L'"',L'№',L';',L'%',L':',L'?',L'*',L'(',L')',L'_',L'+',0,0,L'Й',L'Ц',L'У',L'К',L'Е',L'Н',L'Г',L'Ш',L'Щ',L'З',L'Х',L'Ъ',
			0,0,L'Ф',L'Ы',L'В',L'А',L'П',L'Р',L'О',L'Л',L'Д',L'Ж',L'Э',L'Ё',0,L'/',L'Я',L'Ч',L'С',L'М',L'И',L'Т',L'Ь',L'Б',L'Ю',L',',0,L'*',0,L' ',
		};
		// цифровая клавиатура
		static wchar_t s_nums[13] = {L'7',L'8',L'9',L'-',L'4',L'5',L'6',L'+',L'1',L'2',L'3',L'0',L'.'};

		if (keyEvent < 58) return s_chars[m_bShiftChars][keyEvent];
		else if (keyEvent < 84) {
			if (keyEvent > 70) return s_nums[keyEvent-71];
		} else if (keyEvent == KC_DIVIDE) return '/'; // на цифровой клавиатуре
		else if (keyEvent == KC_OEM_102) return s_chars[m_bShiftChars][KC_BACKSLASH];
		return 0;
	}
}