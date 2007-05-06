#include "MyGUI_StaticText.h"
#include "MyGUI_GUI.h"

using namespace Ogre;
using namespace std;

namespace MyGUI {

	class GUI;

	StaticText::StaticText(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent) :
		Window(lpSkin, uOverlay, pWindowParent)
	{
	}
	
	StaticText *StaticText::create(const Coord &Pos, const Coord &Size,
	    Window *parent, const uint16 uAlign, const uint8 uOverlay, const String &uSkin)
	{
	    __LP_MYGUI_WINDOW_INFO pSkin = SkinManager::getSingleton()->getSkin(uSkin);
		StaticText * pWindow = new StaticText(pSkin->subSkins[0],
		    parent ? OVERLAY_CHILD : uOverlay,
		    parent ? parent->m_pWindowClient : NULL);
		    
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window *pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT) pWindow->m_pWindowText = pChild;
		}
		pWindow->m_uAlign |= (uAlign | WAT_CUT_RIGHT);
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		
		pWindow->move(Pos.x, Pos.y);
		pWindow->size(Size.x > 0 ? Size.x : pSkin->subSkins[0]->sizeX,  
		              Size.y > 0 ? Size.y : pSkin->subSkins[0]->sizeY);
		return pWindow;	
	}
}