#include "MyGUI_StaticImage.h"
#include "MyGUI_GUI.h"
#include "MyGUI_WidgetFactory.h"
#include "MyGUI_Window.h"
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>

using namespace Ogre;

namespace MyGUI {

    StaticImage::StaticImage(const __MYGUI_SUBSKIN_INFO &pSkin, const String & strMaterialElement, uint8 uOverlay, StaticImage *pWindowParent) : 
        m_overlay(NULL),
        m_overlayContainer(NULL),
	    mPosX(pSkin.posX),
	    mPosY(pSkin.posY),
	    mSizeX(pSkin.sizeX),
	    mSizeY(pSkin.sizeY),
	    mParent(pWindowParent)
    {
        mStateSkins[SKIN_STATE_DEACTIVATED]    = pSkin.SkinState[SKIN_STATE_DEACTIVATED];
	    mStateSkins[SKIN_STATE_NORMAL]         = pSkin.SkinState[SKIN_STATE_NORMAL];
	    mStateSkins[SKIN_STATE_ACTIVED]         = pSkin.SkinState[SKIN_STATE_ACTIVED];
	    mStateSkins[SKIN_STATE_PRESSED]        = pSkin.SkinState[SKIN_STATE_PRESSED];
	    mStateSkins[SKIN_STATE_SELECTED]       = pSkin.SkinState[SKIN_STATE_SELECTED];
        
	    //Define a unique label since labels must be unique
	    m_overlayContainer = static_cast<PanelOverlayElement*>(OverlayManager::getSingleton().createOverlayElement(
	        "Panel", "MyGUI_OverlayElement_" + StringConverter::toString((uint32)this))); 
		
	    m_overlayContainer->setMetricsMode(GMM_PIXELS);
	    m_overlayContainer->setPosition(mPosX, mPosY);
	    m_overlayContainer->setDimensions(mSizeX, mSizeY);
		
	    setSkinState(SKIN_STATE_NORMAL);

	    size_t size = GUI::getSingleton()->mRootWindows.size();
	    int16 index = -1;

	    if (uOverlay == OVERLAY_OVERLAPPED) //an overlapped window
	    {
		    //create a unique identifier label
		    m_overlay = GUI::getSingleton()->createOverlay(
		        "MyGUI_Overlay_" + StringConverter::toString((uint32)this), GUI::getSingleton()->m_uMaxZOrder);
			
		    GUI::getSingleton()->m_uMaxZOrder += __GUI_ZORDER_OVERLAPPED_STEP;
		    m_overlay->add2D(m_overlayContainer);

		    // вставка указателя после нижних окон, но выше своих
		    index = (int16) GUI::getSingleton()->m_uOverlappedEnd;
		    GUI::getSingleton()->m_uOverlappedEnd ++;
	    }
	    else if (uOverlay == OVERLAY_CHILD) //a child to some other static image
	    {
		    __ASSERT(mParent != 0);
			mParent->m_overlayContainer->addChild(m_overlayContainer);
			mParent->addChild(this);
	    }
	    else // нет, принадлежит гуям
	    {
		    __ASSERT(uOverlay < __OVERLAY_COUNT); // низя
		    __ASSERT(uOverlay != OVERLAY_MOUSE); // ух ты какой хитрый
		    m_overlay = GUI::getSingleton()->m_overlayGUI[uOverlay];
		    m_overlay->add2D(m_overlayContainer);
			
		    if (m_overlay->getZOrder() < __GUI_ZORDER_OVERLAPPED) { // слои ниже перекрывающихся
			    index = 0;
			    while (index < (int16)GUI::getSingleton()->m_uOverlappedStart) { // поиск места для вставки
				    if (m_overlay->getZOrder() < GUI::getSingleton()->mRootWindows[index]->m_overlay->getZOrder())
				        break;
				    index ++;
			    };
			    GUI::getSingleton()->m_uOverlappedStart ++;
			    GUI::getSingleton()->m_uOverlappedEnd ++;
		    } else { // слои выше перекрывающихся
			    index = (int16)GUI::getSingleton()->m_uOverlappedEnd;
			    while (index < (int16)size) { // поиск места для вставки
				    if (m_overlay->getZOrder() < GUI::getSingleton()->mRootWindows[index]->m_overlay->getZOrder())
				        break;
				    index ++;
			    };
		    }
	    }

	    if (index != -1) { // непосредственно вставка в массив
		    GUI::getSingleton()->mRootWindows.push_back(NULL);
		    for (size_t pos = size; (int16)pos > index; pos--)
		        GUI::getSingleton()->mRootWindows[pos] = GUI::getSingleton()->mRootWindows[pos-1];
		    //TODO: GUI::getSingleton()->mRootWindows[index] = this;
	    }

	    _LOG("create static image (%p)     (%d, %d, %d, %d)",
            this, mPosX, mPosY, mSizeX, mSizeY);

	    show();
    }
    
    StaticImage::~StaticImage()
    {
        //TODO: question: will child overlays get automatically deleted?
        if (m_overlay)
        {
			m_overlay->remove2D(m_overlayContainer);
			//if (m_bIsOverlapped) is commenting this a problem?
                OverlayManager::getSingleton().destroy(m_overlay); //an overlapped window
		}
		else
			m_overlayContainer->getParent()->removeChild(m_overlayContainer->getName());
		
		OverlayManager::getSingleton().destroyOverlayElement(m_overlayContainer);
        
    }
    
    //Static Image
	StaticImage *StaticImage::show(bool ShowWindow)
	{
	    if (ShowWindow)
	        m_overlayContainer->show();
	    else
	        m_overlayContainer->hide();
        
        return this;
	}
	
    StaticImage *StaticImage::hide() //hide the window
	{
	    m_overlayContainer->hide();
	    
	    return this;
    }
	
	StaticImage *StaticImage::setSkinState(__SKIN_STATES Skin)
    {
        if(!mStateSkins[Skin].empty())
            m_overlayContainer->setMaterialName(mStateSkins[Skin]);
        
        return this;
    }
    
    StaticImage *StaticImage::move(int16 iPosX, int16 iPosY) // передвинуть окно
    {
	    mPosX = iPosX;
	    mPosY = iPosY;
	    m_overlayContainer->setPosition(iPosX, iPosY);
	    
	    return this;
    }

    StaticImage *StaticImage::resize(int16 iSizeX, int16 iSizeY) // изменяем размер окна
    {
	    mSizeX = iSizeX;
	    mSizeY = iSizeY;

	    m_overlayContainer->setDimensions(iSizeX, iSizeY);
	    
	    return this;
    }
    
    StaticImage *StaticImage::addChild(StaticImage *child)
    {
        if(child == this) //early out
            return this;
        
        mChildren.push_back(child);
        mChildren.sort();
        mChildren.unique();
        return this;
    }

	// регестрируем фабрику
//	REGISTRY_WIDGET_FACTORY(WIDGET_STATIC_TEXT, StaticText);

} // namespace MyGUI