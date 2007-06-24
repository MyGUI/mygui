#include "MyGUI_GUI.h"
#include "MyGUI_Button.h"
#include "MyGUI_AssetManager.h"
#include "MyGUI_WidgetFactory.h"

using namespace Ogre;
using namespace std;

namespace MyGUI
{

	class GUI;

	Button::Button(const __MYGUI_SUBSKIN_INFO *lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent) :
		Window(lpSkin, strMaterialElement, uOverlay, pWindowParent)
	{
	}

	void Button::_OnMouseSetFocus(MyGUI::Window * pWindowOld) // вызывается при смене активности от курсора
	{
		Window::_OnMouseSetFocus(pWindowOld); // для посылки калбэков
		showFocus(true);
	}

	void Button::_OnMouseLostFocus(MyGUI::Window * pWindowNew) // вызывается при смене активности от курсора
	{
		Window::_OnMouseSetFocus(pWindowNew); // для посылки калбэков
		showFocus(false);
	}

	void Button::_OnMouseButtonPressed(bool bIsLeftButtonPressed) // вызывается при нажатии клавиши
	{
		Window::_OnMouseButtonPressed(bIsLeftButtonPressed); // для посылки калбэков
		showPressed(bIsLeftButtonPressed);
	}

	void Button::onMouseSetFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowOld) // смена фокуса
	{
		if (pWindow->m_uExData & WES_BUTTON) showFocus(true);
	}

	void Button::onMouseLostFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowNew) // смена фокуса
	{
		if (pWindow->m_uExData & WES_BUTTON) showFocus(false);
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
		            Skin = SKIN_STATE_ACTIVED;
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

	Button *Button::createWindow(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin)
	{
		
		const __tag_MYGUI_SKIN_INFO * pSkin = AssetManager::getSingleton()->Skins()->getDefinition(Skin);
		if(!pSkin)
		{
		    _LOG("\n\t[ERROR] Attempting to use a non existant skin \"%s\".  Will set to SKIN_DEFAULT", Skin.c_str());
		    pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_DEFAULT);
		}
		
		Button * pWindow = new Button(pSkin->subSkins[0], pSkin->SkinElement, parent ? OVERLAY_CHILD : uOverlay, parent ? parent->m_pWindowClient : NULL);
		
		pWindow->m_uEventCallback |= WE_MOUSE_BUTTON | WE_KEY_BUTTON;
		for (size_t pos = 1; pos < pSkin->subSkins.size(); ++pos) {
			Window *pChild = new Window(pSkin->subSkins[pos], pSkin->SkinElement, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT)
			    pWindow->setWindowText(pChild);
		}		
		
		pWindow->m_uAlign |= uAlign;
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		pWindow->move(PosX, PosY);
		pWindow->size(SizeX > 0 ? SizeX : pSkin->subSkins[0]->sizeX,  
		              SizeY > 0 ? SizeY : pSkin->subSkins[0]->sizeY);
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		return pWindow;
		
	}

	// регестрируем фабрику
	REGISTRY_WIDGET_FACTORY(WIDGET_BUTTON, Button);

} // namespace MyGUI