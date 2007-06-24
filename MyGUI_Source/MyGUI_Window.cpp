#include "MyGUI_Window.h"
#include "MyGUI_AssetManager.h"
#include "MyGUI_GUI.h"
#include "MyGUI_WidgetFactory.h"
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreFont.h>

using namespace Ogre;
using namespace std;

namespace MyGUI {

	Window::Window(const __MYGUI_SUBSKIN_INFO * lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent) :
		m_overlay(0),
		m_pWindowParent(0),
		m_pEventCallback(GUI::getSingleton()->m_pEventCallback),
		m_uEventCallback(lpSkin->event_info),
		m_iPosX(lpSkin->posX),
		m_iPosY(lpSkin->posY),
		m_iSizeX(lpSkin->sizeX),
		m_iSizeY(lpSkin->sizeY),
		m_iOffsetAlignX(0),
		m_iOffsetAlignY(0),
		m_uAlign(lpSkin->align),
		m_uState(WS_NORMAL),
		m_overlayCaption(0),
		m_sizeTextX(0),
		m_sizeTextY(0),
		m_sizeCutTextX(0),
		m_sizeCutTextY(0),
		m_bIsOverlapped(0),
		m_font(FONT_DEFAULT),
		m_fontColour(1,1,1),
		m_uExData(lpSkin->exdata),
		m_uUserData(0),
		m_lpSkinInfo(lpSkin)
	{
		m_pWindowText = this;
		m_pWindowClient = this;
		
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		
		//a unique name is needed
		m_overlayContainer = static_cast<PanelOverlayElement*>(overlayManager.createOverlayElement(
		    "Panel", "MyGUI_OverlayElement_" + StringConverter::toString((uint32)this))); 
		
		m_overlayContainer->setMetricsMode(GMM_PIXELS);
		m_overlayContainer->setPosition(m_iPosX, m_iPosY);
		m_overlayContainer->setDimensions(m_iSizeX, m_iSizeY);
		
		setSkinState(SKIN_STATE_NORMAL);

		size_t size = GUI::getSingleton()->mRootWindows.size();
		int16 index = -1;

		if (uOverlay == OVERLAY_OVERLAPPED) { // перекрывающееся окно
			m_bIsOverlapped = true; // запоминаем
			m_overlay = GUI::getSingleton()->createOverlay(
			    "MyGUI_Overlay_" + StringConverter::toString((uint32)this), GUI::getSingleton()->m_uMaxZOrder);
			GUI::getSingleton()->m_uMaxZOrder += __GUI_ZORDER_OVERLAPPED_STEP;
			m_overlay->add2D(m_overlayContainer);

			// вставка указателя после нижних окон, но выше своих
			index = (int16) GUI::getSingleton()->m_uOverlappedEnd;
			GUI::getSingleton()->m_uOverlappedEnd ++;
			if (GUI::getSingleton()->m_uMaxZOrder >= __GUI_ZORDER_OVERLAPPED_WARNING) {
				// Многовато окон наклепали
				if (GUI::getSingleton()->m_pEventCallback) GUI::getSingleton()->m_pEventCallback->onWarningEvent(this, WE_WARNING_ZORDER_MAX);
			}
		} else if (uOverlay == OVERLAY_CHILD) { // обыкновенное окно
			__ASSERT(pWindowParent != 0); // низя обманывать
			m_pWindowParent = pWindowParent;
			m_pWindowParent->m_overlayContainer->addChild(m_overlayContainer);
			m_pWindowParent->mChildWindows.push_back(this); // добавляем себя к отцу
			// еще не проверенно , но вроде работает правильно
			if (m_uAlign & WA_CENTER_PARENT) {
				m_iOffsetAlignX = -m_iSizeX;
				m_iOffsetAlignY = -m_iSizeY;
			} else { // Altren - Мир сошел с ума...  (c)
				m_iOffsetAlignX = (m_iPosX << 1) - m_pWindowParent->m_iSizeX;
				m_iOffsetAlignY = (m_iPosY << 1) - m_pWindowParent->m_iSizeY;
			}
			// проверяем высоту нашего бутерброда
			uint uCountParent = 0;
			Window *pWindow = m_pWindowParent;
			while (pWindow) {
				__ASSERT(pWindow != pWindow->m_pWindowParent); // чтобы не зациклило
				pWindow = pWindow->m_pWindowParent;
				uCountParent ++;
			};
			if (uCountParent >= __GUI_ZORDER_OVERLAPPED_STEP) {
				// Многовато детишек наклепали
				if (GUI::getSingleton()->m_pEventCallback) GUI::getSingleton()->m_pEventCallback->onWarningEvent(this, WE_WARNING_CHILD_MAX);
			}
		} else { // нет, принадлежит гуям
			__ASSERT(uOverlay < __OVERLAY_COUNT); // низя
			__ASSERT(uOverlay != OVERLAY_MOUSE); // ух ты какой хитрый
			m_overlay = GUI::getSingleton()->m_overlayGUI[uOverlay];
			m_overlay->add2D(m_overlayContainer);
			if (m_overlay->getZOrder() < __GUI_ZORDER_OVERLAPPED) { // слои ниже перекрывающихся
				index = 0;
				while (index < (int16)GUI::getSingleton()->m_uOverlappedStart) { // поиск места для вставки
					if (m_overlay->getZOrder() < GUI::getSingleton()->mRootWindows[index]->m_overlay->getZOrder()) break;
					index ++;
				};
				GUI::getSingleton()->m_uOverlappedStart ++;
				GUI::getSingleton()->m_uOverlappedEnd ++;
			} else { // слои выше перекрывающихся
				index = (int16)GUI::getSingleton()->m_uOverlappedEnd;
				while (index < (int16)size) { // поиск места для вставки
					if (m_overlay->getZOrder() < GUI::getSingleton()->mRootWindows[index]->m_overlay->getZOrder()) break;
					index ++;
				};
			}
		}

		if (index != -1) // непосредственно вставка в массив
        {
            GUI::getSingleton()->mRootWindows.push_back(NULL);
			for (size_t pos = size; (int16)pos > index; pos--)
			    GUI::getSingleton()->mRootWindows[pos] = GUI::getSingleton()->mRootWindows[pos-1];
			GUI::getSingleton()->mRootWindows[index] = this;
		}

		_LOG("create window (%p)     (%d, %d, %d, %d)   callback(0x%.8X)   "
		     "align(0x%.8X)    overlapped(%d)   data(0x%.8X)", this,
		     m_iPosX, m_iPosY, m_iSizeX, m_iSizeY, m_uEventCallback,
		     m_uAlign, m_bIsOverlapped, m_uExData);

		// устанавливаем нормал скин по дефолту
		if (!strMaterialElement.empty()) {
			m_overlayContainer->setMaterialName(strMaterialElement);
			setSkinState(SKIN_STATE_NORMAL);
		}

		m_overlayContainer->show();
	}

	Window::~Window()
	{
		// удаляем дитешек
		while(!mChildWindows.empty())
		{
		    delete mChildWindows.back();
		    mChildWindows.pop_back();
		}

		_LOG("destroy window (%p)", this);

		// а теперь и сами
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		if (m_overlayCaption) {
			m_overlayContainer->removeChild(m_overlayCaption->getName());
			overlayManager.destroyOverlayElement(m_overlayCaption);
		}

		if (m_overlay) {
			m_overlay->remove2D(m_overlayContainer);
			overlayManager.destroyOverlayElement(m_overlayContainer);
			if (m_bIsOverlapped) overlayManager.destroy(m_overlay); // перекрывающееся окно
		} else {
			__ASSERT(m_pWindowParent != 0); // во как
			m_pWindowParent->m_overlayContainer->removeChild(m_overlayContainer->getName()); // дочернее окно
			overlayManager.destroyOverlayElement(m_overlayContainer);
		}
	}

	void Window::_OnMouseSetFocus(MyGUI::Window * pWindowOld) //The mouse is either now hovering or has stopped hovering over the control
	{
	    //If the callback exists and is listening to MOUSE_FOCUS related messages...
		if (m_pEventCallback && (m_uEventCallback & WE_MOUSE_FOCUS)) {
		    m_pEventCallback->onMouseSetFocus(this, pWindowOld);
		}
	}

	void Window::_OnMouseLostFocus(MyGUI::Window * pWindowNew) //The mouse is either now hovering or has stopped hovering over the control
	{
	    //If the callback exists and is listening to MOUSE_FOCUS related messages...
		if (m_pEventCallback && (m_uEventCallback & WE_MOUSE_FOCUS)) {
			m_pEventCallback->onMouseLostFocus(this, pWindowNew);
		}
	}

	void Window::_OnMouseMove(int16 iPosX, int16 iPosY) // вызывается при движении окна
	{
		//If the callback exists and is listening to mouse movement events...
		if (m_pEventCallback && (m_uEventCallback & WE_MOUSE_MOVE)) {
/*			int16 iParentPosX = iPosX;
			int16 iParentPosY = iPosY;
			
			// относительно отца
			if (m_pWindowParent) {
			    const int Left = int(m_overlayContainer->_getDerivedLeft() * (Real)GUI::getSingleton()->getWidth());
			    const int Top  = int(m_overlayContainer->_getDerivedTop() * (Real)GUI::getSingleton()->getHeight());
				
				iParentPosX -= Left - m_iPosX + 1;
				iParentPosY -= Top - m_iPosY + 1;
			}*/
			
			m_pEventCallback->onMouseMove(this, iPosX, iPosY);
		}
	}

	void Window::_OnMouseButtonPressed(bool bIsLeftButtonPressed) // вызывается при нажатии клавиши
	{
		//if the callback exists and is listening
		if (m_pEventCallback && (m_uEventCallback & WE_MOUSE_BUTTON))
		    m_pEventCallback->onMouseButton(this, bIsLeftButtonPressed);
	}

	void Window::_OnMouseButtonClick(bool bIsDouble) // вызывается при нажатии клавиши и отпускании на том же элементе
	{
	    //if the callback exists and is listening...
		if (m_pEventCallback && (m_uEventCallback & WE_MOUSE_BUTTON))
			if (!bIsDouble)
			    m_pEventCallback->onMouseClick(this);
			else
			    m_pEventCallback->onMouseDoubleClick(this);
	}

	void Window::_OnKeySetFocus(MyGUI::Window * pWindowOld) // вызывается при смене активности ввода
	{
	    //if the callback exists and is listening
		if (m_pEventCallback && (m_uEventCallback & WE_KEY_FOCUS)) {
		    m_pEventCallback->onKeySetFocus(this, pWindowOld);
		}
	}

	void Window::_OnKeyLostFocus(MyGUI::Window * pWindowNew) // вызывается при смене активности ввода
	{
	    //if the callback exists and is listening
		if (m_pEventCallback && (m_uEventCallback & WE_KEY_FOCUS)) {
			m_pEventCallback->onKeyLostFocus(this, pWindowNew);
		}
	}

	void Window::_OnKeyButtonPressed(int keyEvent, wchar_t cText) // вызывается при нажатии клавиши клавы
	{
	    //if the callback exists and is listening
		if (m_pEventCallback && (m_uEventCallback & WE_KEY_BUTTON))
		    m_pEventCallback->onKeyButton(this, keyEvent, cText);
	}

//	void Window::_OnKeyButtonClick(int keyEvent)
//	{
//		if (!m_pEventCallback) return;
//		if (m_uEventCallback&WE_KEY_BUTTON) m_pEventCallback->onKeyClick(this, keyEvent);
//	}

	void Window::show(bool bIsShow) // скрыть показать окно
	{
		if (bIsShow)
		    m_overlayContainer->show();
		else
		    m_overlayContainer->hide();
	}

	void Window::move(int16 iPosX, int16 iPosY) // передвинуть окно
	{
		m_iPosX = iPosX;
		m_iPosY = iPosY;
		m_overlayContainer->setPosition(iPosX, iPosY);
	}

	void Window::size(int16 iSizeX, int16 iSizeY) // изменяем размер окна
	{
		// сначала устанавливаем новые значения, детям посылаем их же
		for (uint i=0; i<mChildWindows.size(); i++) { // сначала детишки
			mChildWindows[i]->alignWindow(iSizeX, iSizeY); // выравниваем
		}
		// а потом только меняем значения класса
		m_iSizeX = iSizeX;
		m_iSizeY = iSizeY;

		m_overlayContainer->setDimensions(iSizeX, iSizeY);
		alignCaption();
	}

	Window *Window::alignWindow(int16 rNewSizeX, int16 rNewSizeY) // выравнивает окно относительно отца
	{
		if (!m_pWindowParent) return this;
		// первоначальное выравнивание 
		if (m_uAlign & WA_RIGHT) {
			if (m_uAlign & WA_LEFT) {
				size(m_iSizeX - (m_pWindowParent->m_iSizeX - rNewSizeX), m_iSizeY); // растягиваем
			}
			else move(m_iPosX - (m_pWindowParent->m_iSizeX - rNewSizeX), m_iPosY); // двигаем
		} else if (!(m_uAlign & WA_LEFT)) { // по горизонтали без растяжения
			move(((rNewSizeX - m_iOffsetAlignX)>>1) + m_iOffsetAlignX, m_iPosY);
		}
		if (m_uAlign & WA_BOTTOM) {
			if (m_uAlign & WA_TOP) {
				size(m_iSizeX, m_iSizeY - (m_pWindowParent->m_iSizeY - rNewSizeY)); // растягиваем
			} else move(m_iPosX, m_iPosY - (m_pWindowParent->m_iSizeY - rNewSizeY)); // двигаем
		} else if (!(m_uAlign & WA_TOP)) { // по вертикали без растяжения
			move(m_iPosX, ((rNewSizeY - m_iOffsetAlignY)>>1) + m_iOffsetAlignY);
		}
		
		return this;

	}

	Window *Window::alignCaption() // выполняет выравнивание текста
	{
		if (!m_overlayCaption)
		    return this;

		if (m_uAlign & __WAT_IS_CUT) {

			if ((m_sizeTextX > (m_iSizeX-__GUI_FONT_SIZE_HOFFSET)) || (m_sizeTextY > m_iSizeY)) { // нужно обрезать
				if (m_strWindowText.empty())
				    m_strWindowText = m_overlayCaption->getCaption(); // текст не был сохранен
				m_sizeCutTextX = m_iSizeX-__GUI_FONT_SIZE_HOFFSET;
				m_sizeCutTextY = m_iSizeY;
				DisplayString strDest;
				
				//Was using this window's font instead of the text window's font
				getCutText(this->m_pWindowText->getFont(),
				    m_sizeCutTextX, m_sizeCutTextY, strDest, m_strWindowText, m_uAlign);
				m_overlayCaption->setCaption(strDest);
				m_overlayCaption->setLeft(__GUI_FONT_HOFFSET);

			} else { // обрезка не нужна
				if (!m_strWindowText.empty()) { // восстанавливаем сохраненный текст
					m_overlayCaption->setCaption(m_strWindowText);
					m_strWindowText = "";
					m_sizeCutTextX = m_sizeTextX;
					m_sizeCutTextY = m_sizeTextY;
				}
			}
	
		}

		if (m_uAlign & WAT_RIGHT) {
			if (m_uAlign & WAT_LEFT) { // по центру
				m_overlayCaption->setLeft((m_iSizeX-m_sizeCutTextX)>>1);
			} else { // по правому краю
				m_overlayCaption->setLeft(m_iSizeX-m_sizeCutTextX-__GUI_FONT_HOFFSET);
			}
		}

		if (m_uAlign & WAT_BOTTOM) {
			if (m_uAlign & WAT_TOP) { // по центру
				m_overlayCaption->setTop((m_iSizeY-m_sizeCutTextY)>>1);
			} else { // по нижнему краю
				m_overlayCaption->setTop(m_iSizeY-m_sizeCutTextY);
			}
		}
		
		return this;

	}

	Window *Window::setCaption(const DisplayString & strText) // устанавливает текст окна
	{
	    m_pWindowText->m_strWindowText = strText;
	    
		if (!m_pWindowText->m_overlayCaption)
		    return this;

		//Is there a reason we are calling the window text's font instead of our own?
		//  are the two supposed to be able to be different?
		//  does a window's font even have meaning except as expressed through its caption?
		getLengthText(m_pWindowText->getFont(), m_pWindowText->m_sizeTextX, m_pWindowText->m_sizeTextY, strText);
		m_pWindowText->m_sizeCutTextX = m_pWindowText->m_sizeTextX;
		m_pWindowText->m_sizeCutTextY = m_pWindowText->m_sizeTextY;
		
		try {
            m_pWindowText->m_overlayCaption->setCaption(strText);
        } catch (...) {
            __ASSERT(false);
        } // недопустимые символы
		
		m_pWindowText->alignCaption();
		return this;
	}

	const DisplayString & Window::getCaption() // возвращает строку окна
	{
		if (m_pWindowText->m_strWindowText.empty() && m_pWindowText->m_overlayCaption)
		    return m_pWindowText->m_overlayCaption->getCaption();
		return m_pWindowText->m_strWindowText;
	}

	void Window::setState(uint8 uState) // состояние окна
	{
		__SKIN_STATES Skin = __SKIN_STATE_COUNT;
		bool bIsShiftText = false;

		if (uState == WS_DEACTIVATED) {
			m_uState = WS_DEACTIVATED;
			Skin = SKIN_STATE_DEACTIVATED;
		} else if (uState == WS_NORMAL) {
			if (m_uState == __WS_SELECTED) {
				m_uState = __WS_ACTIVATED;
				Skin = SKIN_STATE_ACTIVED;
			} else {
				m_uState = WS_NORMAL;
				Skin = SKIN_STATE_NORMAL;
			}
		} else if (uState == WS_PRESSED) {
			bIsShiftText = true;
			if (m_uState == __WS_ACTIVATED) {
				m_uState = __WS_SELECTED;
				Skin = SKIN_STATE_SELECTED;
			} else {
				m_uState = WS_PRESSED;
				Skin = SKIN_STATE_PRESSED;
			}
		}

		if (Skin != __SKIN_STATE_COUNT) { // меняем скины состояний
			// основное окно
			if (m_uExData & WES_BUTTON) {
				setSkinState(Skin);
			}
			// детишки
			for (uint i=0; i<mChildWindows.size(); i++) {
				Window * pChild = mChildWindows[i];
				if (pChild->m_uExData & WES_BUTTON) {
					pChild->setSkinState(Skin);
				}
			}
		}
		
		shiftText(bIsShiftText);
	}

	uint8 Window::getState()  // состояние окна
	{
		if (m_uState == __WS_ACTIVATED)
		    return WS_NORMAL;
		if (m_uState == __WS_SELECTED)
		    return WS_PRESSED;
		return m_uState;
	}
	
	void Window::shiftText(bool bIsShiftText)
	{
	    static bool m_bIsTextShiftPressed = false;
	    
	    if (bIsShiftText != m_bIsTextShiftPressed) { // сдвиг текста
			m_bIsTextShiftPressed = bIsShiftText;
			if (m_pWindowText->m_uAlign & WAT_SHIFT_TEXT) {
				if (m_pWindowText->m_overlayCaption) {
					if (m_bIsTextShiftPressed)
					    m_pWindowText->m_overlayCaption->setTop(m_pWindowText->m_overlayCaption->getTop()
                            + __GUI_BUTTON_SHIFT_TEXT_PRESSED);
					else
					    m_pWindowText->m_overlayCaption->setTop(m_pWindowText->m_overlayCaption->getTop()
					        - __GUI_BUTTON_SHIFT_TEXT_PRESSED);
				}
			}
		}	
	}

	void Window::showFocus(bool bIsFocus) // активирование окна
	{
		if (m_uState == WS_DEACTIVATED) return;
		__SKIN_STATES Skin = __SKIN_STATE_COUNT;

		if (bIsFocus) {
			if (m_uState == WS_NORMAL) {
				m_uState = __WS_ACTIVATED;
				Skin = SKIN_STATE_ACTIVED;
			} else if (m_uState == WS_PRESSED) {
				m_uState = __WS_SELECTED;
				Skin = SKIN_STATE_SELECTED;
			}
		} else {
			if (m_uState == __WS_ACTIVATED) {
				m_uState = WS_NORMAL;
				Skin = SKIN_STATE_NORMAL;
			} else if (m_uState == __WS_SELECTED) {
				m_uState = WS_PRESSED;
				Skin = SKIN_STATE_PRESSED;
			}
		}

		if (Skin != __SKIN_STATE_COUNT) {
			// основное окно
			if (m_uExData & WES_BUTTON) {
				setSkinState(Skin);
			}
			// детишки
			for (uint i=0; i<mChildWindows.size(); i++) {
				Window * pChild = mChildWindows[i];
				if (pChild->m_uExData & WES_BUTTON)
				    pChild->setSkinState(Skin);
			}
		}

	}
	
	void Window::setSkinState(__SKIN_STATES Skin)
	{
		if (!m_lpSkinInfo->SkinState[Skin].empty()) m_overlayContainer->setMaterialName(m_lpSkinInfo->SkinState[Skin]);

		// new version
		if (m_lpSkinInfo->fOffsetStateSkin[Skin][_CX] != 0.0) m_overlayContainer->setUV(
			m_lpSkinInfo->fOffsetStateSkin[Skin][_X],
			m_lpSkinInfo->fOffsetStateSkin[Skin][_Y],
			m_lpSkinInfo->fOffsetStateSkin[Skin][_CX],
			m_lpSkinInfo->fOffsetStateSkin[Skin][_CY]
		);

	}

	bool Window::check(int16 iPosX, int16 iPosY, bool bCapture) // bCapture продолжать ли проверку окон
	{
		if (!m_overlayContainer->isVisible()) return false;
		// если курсор не над окном досвидания
		if (iPosX < m_iPosX) return false;
		if (iPosY < m_iPosY) return false;
		if (iPosX > m_iPosX+m_iSizeX) return false;
		if (iPosY > m_iPosY+m_iSizeY) return false;
		// окно не доступно
		if (m_uState == WS_DEACTIVATED) return true; // чтобы гуи было активно

		if (m_uEventCallback & __WE_IS_ACTION) {
			GUI::getSingleton()->SetCurrentFocusWindow(this);
			 // дочерние не проверяем
//			return true;
		}

		for (uint i=0; i<mChildWindows.size(); i++) {
			if (mChildWindows[i]->check(iPosX-m_iPosX, iPosY-m_iPosY)) return true;
		}

		return bCapture;
	}
	
	const __tag_MYGUI_FONT_INFO *Window::getFont() const
	{
	    return AssetManager::getSingleton()->Fonts()->getDefinition(m_pWindowText->m_font);
    }
    
    Window *Window::setColour(Ogre::ColourValue colour)
    {
        m_pWindowText->m_fontColour = colour;
        m_pWindowText->m_overlayCaption->setColour(colour);
        
        return this;
    }
    
    void Window::bootFont()
    {
        m_pWindowText->m_overlayCaption = static_cast<OverlayContainer*>(
            OverlayManager::getSingleton().createOverlayElement("TextArea", "MyGUI_OverlayElement_TextArea_" + 
            StringConverter::toString((uint32)this)));
			
        m_pWindowText->m_overlayCaption->setMetricsMode(GMM_PIXELS);
		m_pWindowText->m_overlayCaption->setLeft(__GUI_FONT_HOFFSET);
		m_pWindowText->m_overlayContainer->addChild(m_pWindowText->m_overlayCaption);
    }

	Window *Window::setFont(const String &Font, ColourValue colour) // устанавливает шрифт для элемента
	{
	    assert(m_pWindowText);
	    
	    m_pWindowText->m_font = Font;
		
		// элемент не создан, создать
		if (!m_pWindowText->m_overlayCaption)
		    bootFont();
		    
        assert(m_pWindowText->m_overlayCaption);
		
		ParameterList params = m_pWindowText->m_overlayCaption->getParameters();
		m_pWindowText->m_overlayCaption->setParameter("font_name", getFont()->name);
		m_pWindowText->m_overlayCaption->setParameter("char_height", StringConverter::toString(getFont()->height));
//		m_pWindowText->m_overlayCaption->setParameter("space_width", StringConverter::toString(lpFont->spaceWidth));
		setColour(colour);
		m_pWindowText->alignCaption();
		return this;
	}
	
	// возвращает длинну текста
	void Window::getLengthText(const __tag_MYGUI_FONT_INFO *font, int16 &sizeX, int16 &sizeY, const DisplayString & strSource)
	{
		sizeY = font->height;
		sizeX = 0;
		Real len = 0;

		for (DisplayString::const_iterator index = strSource.begin(); index != strSource.end(); index++) {

			if ( index.getCharacter() == ' ') len += font->spaceWidth;
			else if (index.getCharacter() == 10) { // перевод строки

				if (len > sizeX) sizeX = len;
				len = 0;
				sizeY += font->height;

			} else len += font->font->getGlyphAspectRatio(index.getCharacter()) * font->height;
		}

		if (len > sizeX) sizeX = len;
	}

     // возвращает обрезанную строку равную длинне
	void Window::getCutText(const __tag_MYGUI_FONT_INFO *font, int16 &sizeX, int16 &sizeY,
	    DisplayString & strDest, const DisplayString & strSource,
	    uint16 uAlign)
	{
		strDest.clear();
		// строка пустая
		if (strSource.empty()) {sizeY = 0;sizeX = 0;	return;}
		Real len = sizeX;
		float oldLen = len; // сохраняем нам еще пригодится
		// маловато места
		if (sizeX <= 0) {sizeY = 0;sizeX = 0;	return;}
		// даже три точки не помещаются
		bool multiLine = ((uAlign & WAT_MULTI_LINE) != 0);
		bool isEllipses = (((uAlign & WAT_ADD_ELLIPSES) != 0) && (!multiLine));
		if (isEllipses) {
			if (len < font->sizeEllipses) {	sizeX = 0;sizeY = 0;return;	}
			len -= font->sizeEllipses;
		}
		// и высота мала для шрифта
		int16 height = font->height;
		if (sizeY < height) {sizeY = 0;sizeX = 0;return;}

		float sizeChar = 0;
		bool breakWord = ((uAlign & WAT_BREAK_WORD) != 0);

		strDest.resize(strSource.length() + 32);// сразу зададим размер, если будет мало то выкинет
		DisplayString::iterator indexDest = strDest.begin();
		DisplayString::const_iterator indexSourceSpace = strSource.end(); // место последнего пробела
		DisplayString::iterator indexDestSpace = strDest.end(); // место последнего пробела


		if (uAlign & WAT_CUT_RIGHT) { // обрезаем справа

			DisplayString::const_iterator indexSource = strSource.begin();

			while (true) {
				if (indexSource.getCharacter() == ' ') {
					sizeChar = font->spaceWidth;
					indexSourceSpace = indexSource; // запоминаем последний пробел
					indexDestSpace = indexDest; // запоминаем последний пробел
					// а че не присваиваем
				} else if (indexSource.getCharacter() == '\n') { // новая строка
					if ((height + font->height) > sizeY) indexSource = strSource.end(); // выходим из цикла
					else {
						len = sizeX;
						height += font->height;
						indexSourceSpace = strSource.end(); // сбрасываем последний пробел
					}
				} else sizeChar = font->font->getGlyphAspectRatio(indexSource.getCharacter()) * font->height;

				if (sizeChar > len) {
					if (isEllipses) {
						indexDest.setCharacter('.');  indexDest.moveNext();
						indexDest.setCharacter('.');  indexDest.moveNext();
						indexDest.setCharacter('.');  indexDest.moveNext();
					}

					if (multiLine) { // многострочное поле

						if ((height + font->height) > sizeY) indexSource = strSource.end(); // выходим из цикла
						else {
							if (breakWord) { // перенос по словам
								// единственное слово в строке не переносим
								if (indexSourceSpace != strSource.end()) { // возвращаем позицию на последний пробел
									indexSource = indexSourceSpace;
									indexDest = indexDestSpace;
								}
							}
							indexDest.setCharacter('\n');  indexDest.moveNext();
							height += font->height;
							while (indexSource != strSource.end()) { // убираем пробелы вначале новой строки
								if ((indexSource.getCharacter() != ' ') && (indexSource.getCharacter() != '\n')) {
									if (sizeChar > sizeX) {
										// зачем выводить в поле, в котором такая маленькая ширина
										indexSource = strSource.end(); // выходим из цикла
										break;
									}
									indexDest.setCharacter(indexSource.getCharacter());  indexDest.moveNext();
									len = sizeX - sizeChar;
									indexSourceSpace = strSource.end(); // сбрасываем последний пробел
									break;
								};
								indexSource.moveNext();
							};

						}

					} else indexSource = strSource.end(); // выходим из цикла

				} else {
					indexDest.setCharacter(indexSource.getCharacter());  indexDest.moveNext();
					len -= sizeChar;
				}

				if (indexSource != strSource.end()) indexSource.moveNext();
				else break; // выходим из цикла

			};

		} else { // обрезаем слева

			DisplayString::const_iterator indexSource = strSource.end();
			indexSource.movePrev();
			bool exit = false;
			while (true) {
				if (indexSource.getCharacter() == ' ') {
					sizeChar = font->spaceWidth;
					indexSourceSpace = indexSource; // запоминаем последний пробел
				} else if (indexSource.getCharacter() == '\n') { // новая строка
					if ((height + font->height) > sizeY) {
						exit = true;
					} else {
						len = sizeX;
						height += font->height;
						indexSourceSpace = strSource.end(); // сбрасываем последний пробел
					}
				} else sizeChar = font->font->getGlyphAspectRatio(indexSource.getCharacter()) * font->height;

				if (sizeChar > len) exit = true; // выходим
				else len -= sizeChar; // плюсуем символ

				if ((indexSource != strSource.begin()) && (!exit)) indexSource.movePrev(); // слудующая итерация
				else { // хорош крутиться

					if (isEllipses) {
						indexDest.setCharacter('.');  indexDest.moveNext();
						indexDest.setCharacter('.');  indexDest.moveNext();
						indexDest.setCharacter('.');  indexDest.moveNext();
					}

					if (indexSource != strSource.end()) indexSource.moveNext(); // страховка

					// а теперь назад и с копированием
					while (indexSource != strSource.end()) {
						indexDest.setCharacter(indexSource.getCharacter());
						indexDest.moveNext();
						indexSource.moveNext();
					};

					break; // выходим из цикла
				}

			};

		}

		sizeX = oldLen - len;
		sizeY = height;

	}	
	
    Window *Window::createWindow(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	    Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin)
    {
        
		const __tag_MYGUI_SKIN_INFO * pSkin = AssetManager::getSingleton()->Skins()->getDefinition(Skin);
		
		if (!pSkin) {
		    _LOG("\n\t[ERROR] Attempting to use a non existant skin \"%s\".  Will set to SKIN_DEFAULT", Skin.c_str());
		    pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_DEFAULT);
		}
		
		Window * pWindow = new Window(pSkin->subSkins[0],
			pSkin->SkinElement,
		    parent ? OVERLAY_CHILD : uOverlay,
		    parent ? parent->m_pWindowClient : NULL);

		
        
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window *pChild = new Window(pSkin->subSkins[pos], pSkin->SkinElement, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
		}
		
		pWindow->m_uAlign |= uAlign;
		pWindow->move(PosX, PosY);
		pWindow->size(SizeX > 0 ? SizeX : pSkin->subSkins[0]->sizeX,  
		              SizeY > 0 ? SizeY : pSkin->subSkins[0]->sizeY);
        pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		return pWindow;
    }

	MyGUI::Window * Window::createWidget(const String & _type, const String & _skin, uint16 _x, uint16 _y, uint16 _cx, uint16 _cy, uint16 _aligin)
	{
		return GUI::getSingleton()->createFactoryWidget(_type, _skin, _x, _y, _cx, _cy, this, _aligin, 0);
	}

	// регестрируем фабрику
	REGISTRY_WIDGET_FACTORY(WIDGET_WINDOW, Window);

} // namespace MyGUI