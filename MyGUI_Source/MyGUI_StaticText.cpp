#include "MyGUI_StaticText.h"
#include "MyGUI_AssetManager.h"

using namespace Ogre;
using namespace std;

namespace MyGUI {

	StaticText::StaticText(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin)
        
      : Window(AssetManager::getSingleton()->Skins()->getDefinition(Skin)->subSkins[0],
		    parent ? OVERLAY_CHILD : uOverlay,
		    parent ? parent->m_pWindowClient : NULL)	       
    {        
		const __tag_MYGUI_SKIN_INFO * pSkin = AssetManager::getSingleton()->Skins()->getDefinition(Skin);
		
		if(!pSkin)
		{
		    _LOG("\n\t[ERROR] Attempting to use a non existant skin \"%s\".  Will set to SKIN_DEFAULT", Skin.c_str());
		    pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_DEFAULT);
		}
		    
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window *pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, this);
			pChild->m_pEventCallback = (EventCallback*)this;
			if (pChild->m_uExData & WES_TEXT)
			    this->m_pWindowText = pChild;
		}
		
		this->m_uAlign |= (uAlign | WAT_CUT_RIGHT);
		this->setFont(pSkin->fontWindow, pSkin->colour);
		
		this->move(PosX, PosY);
		this->size(SizeX > 0 ? SizeX : pSkin->subSkins[0]->sizeX,  
		           SizeY > 0 ? SizeY : pSkin->subSkins[0]->sizeY);
    }
}