#include "MyGUI_Button.h"
#include "MyGUI_AssetManager.h"

using namespace Ogre;
using namespace std;

namespace MyGUI {

	Button::Button(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin)
      
      : Window(AssetManager::getSingleton()->Skins()->getDefinition(Skin)->subSkins[0],
            parent ? OVERLAY_CHILD           : uOverlay,
            parent ? parent->m_pWindowClient : NULL)
	{
		
		const __tag_MYGUI_SKIN_INFO * pSkin = AssetManager::getSingleton()->Skins()->getDefinition(Skin);
		if(!pSkin)
		{
		    _LOG("\n\t[ERROR] Attempting to use a non existant skin \"%s\".  Will set to SKIN_DEFAULT", Skin.c_str());
		    pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_DEFAULT);
		}
		
		this->m_uEventCallback |= WE_MOUSE_BUTTON | WE_KEY_BUTTON;
		for (size_t pos = 1; pos < pSkin->subSkins.size(); ++pos) {
			Window *pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, this);
			pChild->m_pEventCallback = (EventCallback*)this;
			if (pChild->m_uExData & WES_TEXT)
			    this->setWindowText(pChild);
		}		
		
		this->m_uAlign |= uAlign;
		this->setFont(pSkin->fontWindow, pSkin->colour);
		this->move(PosX, PosY);
		this->size(SizeX > 0 ? SizeX : pSkin->subSkins[0]->sizeX,  
		              SizeY > 0 ? SizeY : pSkin->subSkins[0]->sizeY);
	}

	void Button::_OnMouseChangeFocus(bool bIsFocus) // вызывается при смене активности от курсора
	{
		Window::_OnMouseChangeFocus(bIsFocus); // для посылки калбэков
		showFocus(bIsFocus);
	}

	void Button::_OnMouseButtonPressed(bool bIsLeftButtonPressed) // вызывается при нажатии клавиши
	{
		Window::_OnMouseButtonPressed(bIsLeftButtonPressed); // для посылки калбэков
		showPressed(bIsLeftButtonPressed);
	}

	void Button::onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus) // смена фокуса
	{
		if (pWindow->m_uExData & WES_BUTTON) showFocus(bIsFocus);
	}

	void Button::onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed) // нажата левая кнопка мыши
	{
		if (pWindow->m_uExData & WES_BUTTON)
		    showPressed(bIsLeftButtonPressed);
	}

	void Button::showPressed(bool bIsLeftPressed) // показ но не смена нажатия
	{
		if (m_uState == WS_DEACTIVATED)
		    return;
		
		__SKIN_STATES Skin = __SKIN_STATE_COUNT;
		bool bIsShiftText = false;

		if (bIsLeftPressed) {
		    switch(m_uState)
		    {
		        case WS_NORMAL:
		            Skin = SKIN_STATE_PRESSED;
		            break;
		        case __WS_ACTIVATED:
		            Skin = SKIN_STATE_SELECTED;
		            break;
		        default:
		            return;
			}
			bIsShiftText = true;
		} else {
		    switch(m_uState)
		    {
		        case WS_NORMAL:
		            Skin = SKIN_STATE_NORMAL;
		            break;
		        case __WS_ACTIVATED:
		            Skin = SKIN_STATE_ACTIVE;
		            break;
		        default:
		            return;
		    }
		}

		if (Skin != __SKIN_STATE_COUNT) { // меняем скины состояний
			// основное окно
			if (m_uExData & WES_BUTTON)
				setSkinState(Skin);
			
			//Set all the children windows' graphics to the current state
			for (ChildWindowsIterator i = mChildWindows.begin(); i != mChildWindows.end(); ++i)
				if ((*i)->m_uExData & WES_BUTTON)
				    (*i)->setSkinState(Skin);
		}
        
        shiftText(bIsShiftText);
	}
}